#include "raylib.h"
#include "raymath.h"

#include "background.hpp"
#include "defs.hpp"
#include "lane.hpp"
#include "score.hpp"
#include "sounds.hpp"
#include "sprites.hpp"

#include "entities/pedo.hpp"

const constexpr float WIDTH = 3.0f, HEIGHT = 1.5f, VISUAL_HEIGHT = 10.0f;

const constexpr float FOOTSTEP_DELAY = 0.34f;

const constexpr Color BLOOD{255, 0, 0, 255};
const constexpr float BLOOD_RADIUS = 2.5f, BLOOD_PROB = 0.32f;

const constexpr float AIR_RESISTANCE = 0.92f;

static void bleed(float, float);

Pedo::Pedo(float x, float y) : Entity(x - WIDTH * 0.5f, y, WIDTH, HEIGHT) {}

bool Pedo::is_active() {
    return !Game::active_pedo.expired() && Game::active_pedo.lock().get() == this;
}

void Pedo::update() {
    if (pos.x <= 0.0f) {
        pos.x = 1.0f;
    }

    if (pos.x + width >= Game::width() - 1.0f) {
        pos.x = Game::width() - 2.0f - width;
    }

    if (pos.y <= 0.0f) {
        pos.y = 1.0f;
    }

    if (pos.y + height >= Game::height() - 1.0f) {
        pos.y = Game::height() - 2.0f - height;
    }

    const float safe_zone_hit = SAFE_ZONE + Game::break_lane_width +
                                (Game::front_lanes.size() + Game::back_lanes.size()) * LANE_WIDTH +
                                SAFE_ZONE_DISPATCH_MARGIN;

    if (!safe && pos.x >= safe_zone_hit) {
        Game::active_pedo = {};
        Game::score += dying ? Goals::PEDO_OK : Goals::PEDO_SAVED;
        safe = true;
    }

    if (dying) {
        die_countdown -= TICK_DELAY;

        if (die_countdown <= 0.0f) {
            PlaySound(Sounds::pedo_die);
            deletion_marker = true;

            BeginTextureMode(Game::background.value());
            draw_impl({127, 0, 0, 255});
            EndTextureMode();

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

    if (movement.x > 0.01f) {
        flip_sprite = false;
    } else if (movement.x < -0.01f) {
        flip_sprite = true;
    }

    if (IsKeyDown(KEY_W)) {
        movement.y -= 0.7;
    }

    if (IsKeyDown(KEY_S)) {
        movement.y += 0.7;
    }

    footstep_countdown -= TICK_DELAY;

    if (Vector2LengthSqr(movement) > 0.01f && footstep_countdown <= 0.0f) {
        if (!dying) {
            PlaySound(Sounds::random_footstep());
        }

        footstep_countdown = FOOTSTEP_DELAY;
        footstep_sprite = (footstep_sprite + 1) % 4;
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
    draw_impl(WHITE);
}

void Pedo::draw_impl(Color tint) {
    Texture2D sprite;

    switch (footstep_sprite) {
        case 0:
            sprite = Sprites::Pedo::walking[0];
            break;
        case 1:
        case 3:
            sprite = Sprites::Pedo::standing;
            break;
        case 2:
            sprite = Sprites::Pedo::walking[1];
            break;
    }

    if (dying) {
        DrawTextureEx(
            sprite,
            {
                std::floor(pos.x + width * 0.5f + VISUAL_HEIGHT * 0.5f),
                std::floor(pos.y),
            },
            90.0f, 1.0f, tint
        );
    } else {
        DrawTextureRec(
            sprite,
            {
                0.0f,
                0.0f,
                sprite.width * (flip_sprite ? -1.0f : 1.0f),
                static_cast<float>(sprite.height),
            },
            {
                std::floor(pos.x),
                std::floor(pos.y + height - VISUAL_HEIGHT),
            },
            tint
        );
    }
}

static void bleed(float center_x, float center_y) {
    center_x += WIDTH * 0.5f;
    center_y += HEIGHT * 0.5f;

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
