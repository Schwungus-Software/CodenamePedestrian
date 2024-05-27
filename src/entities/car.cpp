#include "raylib.h"
#include "raymath.h"

#include "car.hpp"
#include "defs.hpp"
#include "lane.hpp"

#include "entities/pedo.hpp"

void Car::update() {
    if (stopping) {
        const float abs = std::max(0.0f, std::abs(vel.y) - SLOWDOWN * TICK_DELAY);
        vel.y = (vel.y >= 0.0 ? abs : -abs);
    }

    apply_velocity();

    const float MARGIN = 32.0f;

    if (pos.y - height - MARGIN > GetScreenHeight() || pos.y + height + MARGIN < 0.0f) {
        deletion_marker = true;
        return;
    }

    if (stopping) {
        despawn_countdown -= TICK_DELAY;

        if (despawn_countdown <= 0.0f) {
            deletion_marker = true;
            return;
        }
    }

    if (Game::active_pedo.expired()) {
        return;
    }

    auto& pedo = *Game::active_pedo.lock();

    if (intersects_with(pedo)) {
        stopping = true;
        despawn_countdown = 5.0f;

        pedo.dying = true;
        pedo.die_countdown = 2.0f;

        pedo.vel = Vector2Add(pedo.vel, vel);
    }
}

void Car::draw() {
    DrawRectangle(pos.x, pos.y, width, height, BLUE);
}

std::optional<std::size_t> Car::get_lane_idx() const {
    if (direction == Direction::FORWARD) {
        for (std::size_t idx = 0; idx < Game::front_lanes.size(); idx++) {
            if (pos.x <= Game::front_lanes[idx].left + LANE_WIDTH * 0.5f) {
                return idx;
            }
        }
    } else {
        for (std::size_t idx = 0; idx < Game::back_lanes.size(); idx++) {
            if (pos.x <= Game::back_lanes[idx].left + LANE_WIDTH * 0.5f) {
                return Game::front_lanes.size() + idx;
            }
        }
    }

    return {};
}
