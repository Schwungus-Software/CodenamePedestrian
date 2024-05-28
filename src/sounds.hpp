#pragma once

#include <array>

#include "raylib.h"

namespace Sounds {
    inline std::array<Sound, 6> footstep;
    inline std::array<Sound, 3> pain;
    inline Sound pedo_die, crash;

    const Sound& random_footstep();
    const Sound& random_pain();
} // namespace Sounds
