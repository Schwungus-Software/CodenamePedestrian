#include "raylib.h"
#include "raymath.h"

#include "background.hpp"
#include "defs.hpp"

#include "entities/pedo.hpp"

const constexpr Color BLOOD{255, 0, 0, 255};
const constexpr int BLOOD_RADIUS = 3;
const constexpr float BLOOD_PROB = 0.3f;

static void bleed(int, int);

bool Pedo::is_active() {
    return !Game::active_pedo.expired() && Game::active_pedo.lock().get() == this;
}

void Pedo::update() {
    if (dying) {
        die_countdown -= TICK_DELAY;

        if (die_countdown <= 0.0f) {
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
    DrawRectangle(pos.x, pos.y, width, VISUAL_HEIGHT, RED);
}

static void bleed(int center_x, int center_y) {
    BeginTextureMode(Game::background.value());

    for (int x = center_x - BLOOD_RADIUS; x <= center_x + BLOOD_RADIUS; x++) {
        for (int y = center_y - BLOOD_RADIUS; y <= center_y + BLOOD_RADIUS; y++) {
            const float prob = 0.01f * GetRandomValue(1, 100);

            if (prob > BLOOD_PROB) {
                continue;
            }

            DrawPixel(x, GetScreenHeight() - 1 - y, BLOOD);
        }
    }

    EndTextureMode();
}
