#include "raylib.h"
#include "raymath.h"

#include "defs.hpp"
#include "lane.hpp"

#include "entities/base.hpp"
#include "entities/pedo.hpp"

extern void reset_background();

static void reset_lanes();
static void reset_pedos();

void restart() {
    reset_lanes();
    Game::entities.clear();
    reset_pedos();

    reset_background();
}

static void reset_lanes() {
    Game::all_pedos_dead = false;

    Game::front_lanes.clear();
    Game::back_lanes.clear();

    const int width = GetScreenWidth() / TOTAL_ZOOM - SAFE_ZONE * 2;

    const int lanes_total_width = width - BREAK_LANE_MIN_WIDTH;
    const int lanes_count = lanes_total_width / LANE_WIDTH;

    Game::no_lanes_bitch = lanes_count < 1;

    if (Game::no_lanes_bitch) {
        return;
    }

    Game::break_lane_width = width - lanes_count * LANE_WIDTH;

    const int front_lanes = std::max(1, lanes_count / 2);
    const int back_lanes = std::max(0, lanes_count - front_lanes);

    for (std::size_t i = 0; i < front_lanes; i++) {
        Game::front_lanes.push_back({
            .left = static_cast<int>(SAFE_ZONE + i * LANE_WIDTH),
            .width = LANE_WIDTH,
        });
    }

    const auto shebang = Game::front_lanes.back().left + LANE_WIDTH + Game::break_lane_width;

    for (std::size_t i = 0; i < back_lanes; i++) {
        Game::back_lanes.push_back({
            .left = static_cast<int>(shebang + i * LANE_WIDTH),
            .width = LANE_WIDTH,
        });
    }
}

static void reset_pedos() {
    for (std::size_t i = 0; i < PEDO_COUNT; i++) {
        const Vector2 origin{SAFE_ZONE / 2.0f, (i + 1) * PEDO_SPAWN_RADIUS * 2.01f};
        // TODO: randomize offset.
        const Vector2 offset{0.0f, 0.0f};

        const auto pos = Vector2Add(origin, offset);

        Game::entities.emplace_back(new Pedo(pos.x, pos.y));
    }
}
