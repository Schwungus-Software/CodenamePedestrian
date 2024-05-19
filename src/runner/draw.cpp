#include "raylib.h"

#include "camera.hpp"
#include "defs.hpp"

#include "entities/base.hpp"

static void draw_entities() {
    for (auto& entity : Game::entities) {
        entity->draw();
    }
}

static void draw_lane(float left) {
    DrawRectangle(left, 0.0, LANE_WIDTH, WORLD_HEIGHT, DARKGRAY);
    // TODO: draw the white stripe thingies, i don't know what they are because i don't have a
    // driver's license.
}

void draw() {
    BeginDrawing();

    ClearBackground(GREEN);

    BeginMode2D({
        .offset =
            {
                GetScreenWidth() / 2.0f,
                GetScreenHeight() / 2.0f,
            },
        .target = Game::look_at,
        .rotation = 0.0,
        .zoom = 4.0,
    });

    draw_entities();

    EndMode2D();
    EndDrawing();
}
