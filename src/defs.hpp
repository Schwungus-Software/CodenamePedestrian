#pragma once

#include <cstddef>

const constexpr int TICK_RATE = 60;
const constexpr std::size_t PEDO_COUNT = 20;

const constexpr float TICK_DELAY = 1.0f / TICK_RATE, LANE_WIDTH = 64.0f, LANE_BREAK_WIDTH = 24.0f,
                      WORLD_HEIGHT = 640.0f, PEDO_WIDTH = 4.0f, PEDO_VISUAL_HEIGHT = 9.0f,
                      PEDO_SPAWN_RADIUS = 3.0;
