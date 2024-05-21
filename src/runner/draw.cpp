#include "raylib.h"

#include "defs.hpp"
#include "lane.hpp"

#include "entities/base.hpp"

static void draw_entities() {
    for (auto& entity : Game::entities) {
        entity->draw();
    }
}

static void draw_driving_lane(float left, float width) {
    DrawRectangle(left, 0.0f, width, GetScreenHeight(), DARKGRAY);
    // TODO: draw the white stripe thingies, i don't know what they are because i don't have a
    // driver's license.
}

static void draw_safe_lane(float left, float width) {
    DrawRectangle(left, 0.0f, width, GetScreenHeight(), LIGHTGRAY);
}

static void draw_lanes() {
    draw_safe_lane(0.0f, SAFE_ZONE);
    float left_cumulative = SAFE_ZONE;

    for (const auto& lane : Game::front_lanes) {
        draw_driving_lane(lane.left, lane.width);
        left_cumulative += LANE_WIDTH;
    }

    draw_safe_lane(left_cumulative, Game::break_lane_width);
    left_cumulative += Game::break_lane_width;

    for (const auto& lane : Game::back_lanes) {
        draw_driving_lane(lane.left, lane.width);
        left_cumulative += LANE_WIDTH;
    }

    draw_safe_lane(left_cumulative, SAFE_ZONE);
}

void draw() {
    BeginDrawing();

    if (Game::no_lanes_bitch) {
        ClearBackground(RED);
        DrawText("NO LANES; WINDOW TOO SMALL", 10, 10, 30, WHITE); // TODO: center
        EndDrawing();
        return;
    }

    ClearBackground(GREEN);

    BeginMode2D({
        .offset = {0.0f, 0.0f},
        .target = {0.0f, 0.0f},
        .rotation = 0.0f,
        .zoom = TOTAL_ZOOM,
    });

    draw_lanes();
    draw_entities();

    EndMode3D();
    EndDrawing();
}
