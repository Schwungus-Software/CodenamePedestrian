#pragma once

#include <cstddef>

#include "raylib.h"

const constexpr int TICK_RATE = 60;
const constexpr float TICK_DELAY = 1.0f / TICK_RATE;

const constexpr float TOTAL_ZOOM = 4.0f;

const constexpr std::size_t PEDO_COUNT = 20;
const constexpr float CARS_PER_LANE = 1.2f;

const constexpr int SAFE_ZONE = 32, SAFE_ZONE_DISPATCH_MARGIN = 4;
const constexpr int LANE_WIDTH = 24, BREAK_LANE_MIN_WIDTH = 12;

namespace Game {
    inline const float width() {
        return GetScreenWidth() / TOTAL_ZOOM;
    }

    inline const float height() {
        return GetScreenHeight() / TOTAL_ZOOM;
    }
} // namespace Game
