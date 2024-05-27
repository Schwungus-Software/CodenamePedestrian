#include <memory>
#include <set>

#include "raylib.h"

#include "defs.hpp"
#include "lane.hpp"

#include "entities/base.hpp"
#include "entities/car.hpp"
#include "entities/pedo.hpp"

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

            if (ptr != nullptr && !ptr->safe) {
                Game::active_pedo = ptr;
                break;
            }
        }
    }

    if (Game::active_pedo.expired()) {
        Game::all_pedos_gone = true;
    }
}

const constexpr float BASE_SPAWN_DELAY_PER_LANE = 2.0f, SPAWN_DELAY_RANDOM_OFFSET = 5.0f;

static void spawn_cars() {
    static float spawn_delay = 0.0f;
    spawn_delay -= TICK_DELAY;

    if (spawn_delay > 0.0f) {
        return;
    }

    const auto lanes_count = Game::front_lanes.size() + Game::back_lanes.size();
    std::set<std::size_t> occupied_lanes;

    for (const auto& entity : Game::entities) {
        const auto& car = std::dynamic_pointer_cast<Car>(entity);

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
        const float randomness = GetRandomValue(1, 100) * 0.01f;
        spawn_delay = BASE_SPAWN_DELAY_PER_LANE + SPAWN_DELAY_RANDOM_OFFSET * randomness;
        spawn_delay /= lanes_count;

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
