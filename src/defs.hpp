#pragma once

#include <cstddef>

const constexpr int TICK_RATE = 60;
const constexpr std::size_t PEDO_COUNT = 20;

const constexpr float TICK_DELAY = 1.0f / TICK_RATE, TOTAL_ZOOM = 4.0f, PEDO_SPAWN_RADIUS = 3.0f,
                      CARS_PER_LANE = 1.2f, CAR_SPAWN_DELAY = 2.0f;

const constexpr int SAFE_ZONE = 32, LANE_WIDTH = 24, BREAK_LANE_MIN_WIDTH = 12;
