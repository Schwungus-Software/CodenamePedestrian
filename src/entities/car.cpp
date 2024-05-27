#include "raylib.h"
#include "raymath.h"

#include "background.hpp"
#include "car.hpp"
#include "defs.hpp"
#include "lane.hpp"
#include "sounds.hpp"

#include "entities/pedo.hpp"

const constexpr float PEDO_DEATH_DELAY = 2.0f, CAR_DESPAWN_DELAY = 5.0f;
const constexpr float CRASH_IMPULSE_FACTOR = 1.7f, CRASH_HORIZONTAL_IMPULSE = 64.0f;

const constexpr Color BRAKE_TRAIL{12, 12, 12, 255};
const constexpr float BRAKE_TRAIL_WIDTH = 1.5f;

static void draw_brake_trail(float, float, float, float);

void Car::update() {
    if (braking) {
        const float abs = std::max(0.0f, std::abs(vel.y) - SLOWDOWN * TICK_DELAY);
        vel.y = (vel.y >= 0.0 ? abs : -abs);
    }

    const auto old_pos = pos;
    apply_velocity();

    if (braking) {
        const auto dist = Vector2Subtract(pos, old_pos).y;
        draw_brake_trail(old_pos.x, old_pos.y, width, dist);
    }

    const float MARGIN = 32.0f;

    if (pos.y - height - MARGIN > GetScreenHeight() || pos.y + height + MARGIN < 0.0f) {
        deletion_marker = true;
        return;
    }

    if (braking) {
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

    if (!pedo.dying && intersects_with(pedo)) {
        if (!braking) {
            braking = true;
            despawn_countdown = CAR_DESPAWN_DELAY;
        }

        pedo.dying = true;
        pedo.die_countdown = PEDO_DEATH_DELAY;

        pedo.vel = Vector2Add(pedo.vel, Vector2Scale(vel, CRASH_IMPULSE_FACTOR));
        pedo.vel.x += CRASH_HORIZONTAL_IMPULSE * GetRandomValue(-50, 50) * 0.02f;

        PlaySound(Sounds::crash);
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

static void draw_brake_trail(float x, float y, float car_width, float dist) {
    const float margin = 1.0f;

    BeginTextureMode(Game::background.value());

    if (dist < 0.0f) {
        dist = std::abs(dist);
        y -= dist;
    }

    DrawRectangle(x + margin, y, BRAKE_TRAIL_WIDTH, dist, BRAKE_TRAIL);
    DrawRectangle(x + car_width - margin, y, BRAKE_TRAIL_WIDTH, dist, BRAKE_TRAIL);

    EndTextureMode();
}
