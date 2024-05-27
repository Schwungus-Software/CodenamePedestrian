#include <memory>
#include <set>

#include "raylib.h"

#include "defs.hpp"
#include "lane.hpp"

#include "entities/base.hpp"
#include "entities/car.hpp"
#include "entities/pedo.hpp"

const constexpr float CAR_SPAWN_DELAY = 0.8f;

extern void restart();

static void spawn_cars();

void update() {
    if (IsKeyPressed(KEY_R)) {
        restart();
        return;
    }

    spawn_cars();

    for (auto& entity : Game::entities) {
        entity->update();
    }

    std::erase_if(Game::entities, [](const auto& x) { return x->deletion_marker; });

    if (Game::active_pedo.expired()) {
        for (auto& entity : Game::entities) {
            auto ptr = std::dynamic_pointer_cast<Pedo>(entity);

            if (ptr != nullptr) {
                Game::active_pedo = ptr;
                break;
            }
        }
    }
}

static void spawn_cars() {
    static float last_spawn = GetTime();
    const float cur_time = GetTime();

    if (cur_time - last_spawn > CAR_SPAWN_DELAY) {
        last_spawn = cur_time;
    } else {
        return;
    }

    const auto lanes_count = Game::front_lanes.size() + Game::back_lanes.size();
    std::set<std::size_t> occupied_lanes;

    for (const auto& entity : Game::entities) {
        const auto car = std::dynamic_pointer_cast<Car>(entity);

        if (car != nullptr) {
            const auto lane_idx = car->get_lane_idx();

            if (lane_idx.has_value()) {
                occupied_lanes.insert(lane_idx.value());
            }
        }
    }

    std::size_t max_attempts = 100, lane_idx;

    do {
        lane_idx = GetRandomValue(0, lanes_count - 1);
    } while (--max_attempts > 0 && occupied_lanes.contains(lane_idx));

    if (max_attempts > 0) {
        const float WIDTH = LANE_WIDTH * 0.9f, HEIGHT = LANE_WIDTH * 1.8f;

        float left;
        Car::Direction dir;

        if (lane_idx >= Game::front_lanes.size()) {
            left = Game::back_lanes[lane_idx - Game::front_lanes.size()].left;
            dir = Car::Direction::BACKWARD;
        } else {
            left = Game::front_lanes[lane_idx].left;
            dir = Car::Direction::FORWARD;
        }

        left += (LANE_WIDTH - WIDTH) * 0.5f;

        Game::entities.emplace_back(new Car(left, WIDTH, HEIGHT, 512.0f, dir));
    }
}
