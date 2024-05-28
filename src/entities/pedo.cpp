#include "raylib.h"
#include "raymath.h"

#include "background.hpp"
#include "defs.hpp"
#include "lane.hpp"
#include "score.hpp"
#include "sounds.hpp"

#include "entities/pedo.hpp"

const constexpr float FOOTSTEP_DELAY = 0.34f;

const constexpr Color BLOOD{255, 0, 0, 255};
const constexpr float BLOOD_RADIUS = 2.5f, BLOOD_PROB = 0.32f;

static void bleed(float, float);

bool Pedo::is_active() {
    return !Game::active_pedo.expired() && Game::active_pedo.lock().get() == this;
}

void Pedo::update() {
    if (pos.x < 0.0f) {
        pos.x = 0.0f;
    } else if (pos.x + width >= GetScreenWidth() - 1.0f) {
        pos.x = GetScreenWidth() - 1.0f - width;
    }

    if (pos.y < 0.0f) {
        pos.y = 0.0f;
    } else if (pos.y + height >= GetScreenHeight() - 1.0f) {
        pos.y = GetScreenHeight() - 1.0f - height;
    }

    if (safe) {
        apply_velocity();
        return;
    }

    const float safe_zone_hit = SAFE_ZONE + Game::break_lane_width +
                                (Game::front_lanes.size() + Game::back_lanes.size()) * LANE_WIDTH +
                                SAFE_ZONE_DISPATCH_MARGIN;

    if (!dying && pos.x >= safe_zone_hit) {
        Game::active_pedo = {};
        Game::score += Goals::PEDO_SAVED;
        dying = false;
        safe = true;
        return;
    }

    if (dying) {
        die_countdown -= TICK_DELAY;

        if (die_countdown <= 0.0f) {
            PlaySound(Sounds::pedo_die);
            deletion_marker = true;
            return;
        }
    }

    if (!is_active()) {
        return;
    }

    const Vector2 old_pos = pos;

    Vector2 movement{0.0f, 0.0f};
    const float MOVE_SPEED = 8.0f;

    if (IsKeyDown(KEY_A)) {
        movement.x -= 1.0;
    }

    if (IsKeyDown(KEY_D)) {
        movement.x += 1.0;
    }

    if (IsKeyDown(KEY_W)) {
        movement.y -= 0.7;
    }

    if (IsKeyDown(KEY_S)) {
        movement.y += 0.7;
    }

    footstep_countdown -= TICK_DELAY;

    if (!dying && Vector2LengthSqr(movement) > 0.01f && footstep_countdown <= 0.0f) {
        PlaySound(Sounds::random_footstep());
        footstep_countdown = FOOTSTEP_DELAY;
    }

    movement = Vector2Scale(movement, TICK_DELAY * MOVE_SPEED);
    pos = Vector2Add(pos, movement);

    vel = Vector2Scale(vel, AIR_RESISTANCE);
    apply_velocity();

    const auto dist = Vector2Subtract(pos, old_pos);
    const auto center = Vector2Add(old_pos, Vector2Scale(dist, 0.5f));

    if (dying) {
        bleed(old_pos.x, old_pos.y);
        bleed(center.x, center.y);
        bleed(pos.x, pos.y);
    }
}

void Pedo::draw() {
    const auto color = RED;

    if (dying) {
        DrawRectangle(
            pos.x + width * 0.5f - VISUAL_HEIGHT * 0.5f, pos.y, VISUAL_HEIGHT, width, color
        );
    } else {
        DrawRectangle(pos.x, pos.y + height - VISUAL_HEIGHT, width, VISUAL_HEIGHT, color);
    }
}

static void bleed(float center_x, float center_y) {
    center_x += Pedo::WIDTH * 0.5f;
    center_y += Pedo::HEIGHT * 0.5f;

    BeginTextureMode(Game::background.value());

    for (float x = center_x - BLOOD_RADIUS; x <= center_x + BLOOD_RADIUS; x += 1.0f) {
        for (float y = center_y - BLOOD_RADIUS; y <= center_y + BLOOD_RADIUS; y += 1.0f) {
            const float prob = 0.01f * GetRandomValue(1, 100);

            if (prob > BLOOD_PROB) {
                continue;
            }

            DrawPixel(x, y, BLOOD);
        }
    }

    EndTextureMode();
}
