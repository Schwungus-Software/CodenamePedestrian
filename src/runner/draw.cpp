#include <optional>
#include <string>

#include "raylib.h"

#include "background.hpp"
#include "defs.hpp"
#include "lane.hpp"
#include "score.hpp"

#include "entities/base.hpp"
#include "entities/pedo.hpp"

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

void reset_background() {
    if (Game::background.has_value()) {
        UnloadRenderTexture(Game::background.value());
    }

    Game::background = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

    BeginTextureMode(Game::background.value());
    ClearBackground(GREEN);

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

    EndTextureMode();
}

void draw() {
    BeginDrawing();

    const auto center_err = [](const char* text) {
        ClearBackground(RED);

        const int fs = 30;
        const auto width = MeasureText(text, fs);

        DrawText(
            text, 0.5f * (GetScreenWidth() - width), 0.5f * (GetScreenHeight() - fs), fs, WHITE
        );

        EndDrawing();
    };

    const auto draw_score = []() {
        const int score_width = 90, score_margin = 10,
                  score_left = GetScreenWidth() - score_width - score_margin, score_fs = 30;

        const char* prefix = "SCORE: ";
        const Color color = BLACK;

        DrawText(prefix, score_left - MeasureText(prefix, score_fs), score_margin, score_fs, color);
        DrawText(std::to_string(Game::score).c_str(), score_left, score_margin, score_fs, color);
    };

    if (Game::no_lanes_bitch) {
        center_err("NO LANES; WINDOW TOO SMALL");
        return;
    }

    if (Game::all_pedos_gone) {
        if (Game::score > 0) {
            center_err("YOU SAVED SOMEONE! PRESS R TO TRY AGAIN");
        } else {
            center_err("GAME OVER. PRESS R TO RESTART");
        }

        draw_score();

        return;
    }

    ClearBackground(BLACK);

    BeginMode2D({
        .offset = {0.0f, 0.0f},
        .target = {0.0f, 0.0f},
        .rotation = 0.0f,
        .zoom = TOTAL_ZOOM,
    });

    const auto& bg = Game::background.value().texture;

    DrawTextureRec(
        bg,
        {
            .x = 0.0f,
            .y = 0.0f,
            .width = static_cast<float>(bg.width),
            .height = static_cast<float>(-bg.height),
        },
        {0.0f, 0.0f}, WHITE
    );

    draw_entities();

    EndMode2D();

    draw_score();

    EndDrawing();
}
