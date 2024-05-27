#pragma once

#include <array>

#include "raylib.h"

namespace Sounds {
    inline std::array<Sound, 6> footstep;
    inline Sound pedo_die, crash;

    const Sound& random_footstep();
} // namespace Sounds
