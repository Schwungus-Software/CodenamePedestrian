#include <optional>
#include <string>

#include "raylib.h"

#include "background.hpp"
#include "defs.hpp"
#include "lane.hpp"
#include "score.hpp"

#include "entities/base.hpp"
#include "entities/pedo.hpp"

constexpr const int GRID_CELL_SIZE = 4;

static void draw_entities() {
    for (auto& entity : Game::entities) {
        entity->draw();
    }
}

static void draw_driving_lane(int left, int width) {
    DrawRectangle(left, 0.0f, width, Game::height(), DARKGRAY);
    DrawLine(left + width, 0.0f, left + width, Game::height(), WHITE);
}

static void draw_safe_lane(int left, int width) {
    const Color TILE_COLOR{240, 240, 240, 255}, GRID_COLOR{220, 220, 220, 255};

    DrawRectangle(left, 0, width, Game::height(), TILE_COLOR);

    for (int gx = left; gx <= left + width; gx += GRID_CELL_SIZE) {
        DrawLine(gx, 0, gx, Game::height(), GRID_COLOR);
    }

    for (int gy = 0; gy <= Game::height(); gy += GRID_CELL_SIZE) {
        DrawLine(left, gy, left + width, gy, GRID_COLOR);
    }
}

void reset_background() {
    if (Game::background.has_value()) {
        UnloadRenderTexture(Game::background.value());
    }

    Game::background = LoadRenderTexture(Game::width(), Game::height());

    BeginTextureMode(Game::background.value());
    ClearBackground(GREEN);

    draw_safe_lane(0, SAFE_ZONE);
    int left_cumulative = SAFE_ZONE;

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
