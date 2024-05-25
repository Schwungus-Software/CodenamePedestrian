#include "raylib.h"

#include "car.hpp"
#include "defs.hpp"
#include "lane.hpp"

#include "entities/pedo.hpp"

void Car::update() {
    if (stopping) {
        vel.y = std::max(0.0f, vel.y - SLOWDOWN * TICK_DELAY);
    }

    apply_velocity();

    const float MARGIN = 32.0f;

    if (pos.y - height - MARGIN > GetScreenHeight() || pos.y + height + MARGIN < 0.0f) {
        deletion_marker = true;
    }

    if (Game::active_pedo.expired()) {
        return;
    }

    auto& pedo = *Game::active_pedo.lock();

    if (intersects_with(pedo)) {
        stopping = true;

        pedo.dying = true;
        pedo.die_countdown = 3.0f;
    }
}

void Car::draw() {
    DrawRectangle(pos.x, pos.y, width, height, BLUE);
}

std::optional<std::size_t> Car::get_lane_idx() const {
    const float EPSILON = 0.1f;

    if (direction == Direction::FORWARD) {
        for (std::size_t idx = 0; idx < Game::front_lanes.size(); idx++) {
            if (pos.x <= Game::front_lanes[idx].left + EPSILON) {
                return idx;
            }
        }
    } else {
        for (std::size_t idx = 0; idx < Game::back_lanes.size(); idx++) {
            if (pos.x <= Game::back_lanes[idx].left + EPSILON) {
                return Game::front_lanes.size() + idx;
            }
        }
    }

    return {};
}
