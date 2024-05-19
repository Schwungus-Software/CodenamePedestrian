#pragma once

#include <cstddef>

const constexpr int FRAMERATE = 60;
const constexpr std::size_t PEDO_COUNT = 20;

const constexpr float TICKRATE = 1.0f / FRAMERATE, LANE_WIDTH = 64.0f, LANE_BREAK_WIDTH = 24.0f,
                      WORLD_HEIGHT = 640.0f, PEDO_WIDTH = 5.0f, PEDO_VISUAL_HEIGHT = 16.0f,
                      PEDO_SPAWN_RADIUS = 3.0;
