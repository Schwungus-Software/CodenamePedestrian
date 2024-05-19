#include "raylib.h"
#include "raymath.h"

#include "defs.hpp"

#include "entities/base.hpp"
#include "entities/pedo.hpp"

void restart() {
    Game::entities.clear();

    for (std::size_t i = 0; i < PEDO_COUNT; i++) {
        const Vector2 origin{PEDO_WIDTH, (i + 1) * PEDO_SPAWN_RADIUS * 2.01f};
        // TODO: randomize offset.
        const Vector2 offset{0.0f, 0.0f};

        const auto pos = Vector2Add(origin, offset);

        Game::entities.emplace_back(new Pedo(pos.x, pos.y));
    }
}
