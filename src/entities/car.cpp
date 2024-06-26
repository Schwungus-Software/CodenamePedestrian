#include <cmath>

#include "raylib.h"
#include "raymath.h"

#include "background.hpp"
#include "car.hpp"
#include "defs.hpp"
#include "lane.hpp"
#include "sounds.hpp"
#include "sprites.hpp"

#include "entities/pedo.hpp"

const constexpr float CAR_VELOCITY = 128.0f, CAR_RANDOM = 312.0f;
const constexpr float CAR_ACCEL = 212.0f, CAR_SLOWDOWN = 864.0f;

const constexpr float PEDO_DEATH_DELAY = 5.0f, CAR_REALIZATION_DELAY = 3.0f;
const constexpr float CRASH_IMPULSE_FACTOR = 1.7f, CRASH_HORIZONTAL_IMPULSE = 312.0f;

const constexpr Color BRAKE_TRAIL{12, 12, 12, 255};
const constexpr float BRAKE_TRAIL_WIDTH = 1.5f;

static void draw_brake_trail(float, float, float, float, float);

Car::Car(float left, float width, float height, Direction direction)
    : Entity(left, 0.0f, width, height), direction(direction) {
    const float fdir = (direction == Direction::FORWARD ? 1.0f : -1.0f);
    const float random = GetRandomValue(0, 100) / 100.0f;

    vel.y = (CAR_VELOCITY + random * CAR_RANDOM) * fdir;
    starting_velocity = std::abs(vel.y);

    if (direction == Direction::FORWARD) {
        pos.y = -height + 1.0f;
    } else {
        pos.y = Game::height() - 2.0f;
    }
}

void Car::update() {
    float abs;

    if (braking) {
        abs = std::max(0.0f, std::abs(vel.y) - CAR_SLOWDOWN * TICK_DELAY);
    } else {
        abs = std::min(starting_velocity, std::abs(vel.y) + CAR_ACCEL * TICK_DELAY);
    }

    vel.y = (direction == Direction::FORWARD ? abs : -abs);

    const auto old_pos = pos;
    apply_velocity();

    if (braking) {
        const auto dist = Vector2Subtract(pos, old_pos).y;
        draw_brake_trail(old_pos.x, old_pos.y, width, height, dist);
    }

    const float MARGIN = 32.0f;

    if ((direction == Direction::FORWARD && pos.y - height - MARGIN > Game::height()) ||
        (direction == Direction::BACKWARD && pos.y + height + MARGIN < 0.0f)) {
        deletion_marker = true;
        return;
    }

    if (braking) {
        realization_countdown -= TICK_DELAY;

        if (realization_countdown <= 0.0f) {
            braking = false;
            return;
        }
    }

    if (Game::active_pedo.expired()) {
        return;
    }

    auto& pedo = *Game::active_pedo.lock();

    if (intersects_with(pedo)) {
        if (!braking) {
            realization_countdown = CAR_REALIZATION_DELAY;
        }

        braking = true;

        if (pedo.dying) {
            pedo.die_countdown = 0.0f;
        } else {
            pedo.dying = true;
            pedo.die_countdown = PEDO_DEATH_DELAY;

            pedo.vel = Vector2Add(pedo.vel, Vector2Scale(vel, CRASH_IMPULSE_FACTOR));
            pedo.vel.x += CRASH_HORIZONTAL_IMPULSE * GetRandomValue(-50, 50) * 0.02f;
            pedo.apply_velocity();
        }

        PlaySound(Sounds::crash);
    }
}

void Car::draw() {
    DrawTextureRec(
        Sprites::car,
        {
            0.0f,
            0.0f,
            width,
            direction == Direction::FORWARD ? height : -height,
        },
        {
            std::floor(pos.x),
            std::floor(pos.y),
        },
        WHITE
    );
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

static void draw_brake_trail(float x, float y, float car_width, float car_height, float dist) {
    const float margin = 1.0f;

    BeginTextureMode(Game::background);

    if (dist < 0.0f) {
        dist = std::abs(dist);
        y -= dist;
        y += car_height;
    }

    DrawRectangle(x + margin, y, BRAKE_TRAIL_WIDTH, dist, BRAKE_TRAIL);
    DrawRectangle(x + car_width - 1.0f - margin, y, BRAKE_TRAIL_WIDTH, dist, BRAKE_TRAIL);

    EndTextureMode();
}
