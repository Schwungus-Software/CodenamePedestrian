#pragma once

#include <vector>

struct Lane {
    int left, width;
};

namespace Game {
    inline bool no_lanes_bitch = false;
    inline int break_lane_width;
    inline std::vector<Lane> front_lanes, back_lanes;
} // namespace Game
