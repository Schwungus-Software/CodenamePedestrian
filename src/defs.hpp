#pragma once

#include <cstddef>

const constexpr int TICK_RATE = 60;
const constexpr std::size_t PEDO_COUNT = 20;

const constexpr float TICK_DELAY = 1.0f / TICK_RATE, TOTAL_ZOOM = 4.0f, SAFE_ZONE = 32.0f,
                      LANE_WIDTH = 24.0f, BREAK_LANE_MIN_WIDTH = 12.0f, PEDO_SPAWN_RADIUS = 3.0f;
