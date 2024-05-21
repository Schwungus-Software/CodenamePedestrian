#pragma once

#include <vector>

struct Lane {
    float left, width;
};

namespace Game {
    inline bool no_lanes_bitch = false;
    inline float break_lane_width;
    inline std::vector<Lane> front_lanes, back_lanes;
} // namespace Game
