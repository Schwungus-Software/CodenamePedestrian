#include "raylib.h"
#include "raymath.h"

#include "defs.hpp"

#include "entities/pedo.hpp"

bool Pedo::is_active() {
    return !Game::active_pedo.expired() && Game::active_pedo.lock().get() == this;
}

void Pedo::update() {
    if (!is_active()) {
        return;
    }

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

    apply_velocity();
}

void Pedo::draw() {
    DrawRectangle(pos.x, pos.y, width, VISUAL_HEIGHT, RED);
}
