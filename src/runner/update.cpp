#include <memory>

#include "camera.hpp"
#include "defs.hpp"

#include "entities/base.hpp"
#include "entities/pedo.hpp"

void update() {
    for (auto& entity : Game::entities) {
        entity->update();
    }

    if (Game::active_pedo.expired()) {
        for (auto& entity : Game::entities) {
            try {
                Game::active_pedo = std::dynamic_pointer_cast<Pedo>(entity);
                break;
            } catch (const std::bad_cast&) {
                // skip
            }
        }
    }

    if (!Game::active_pedo.expired()) {
        const auto& pedo = *Game::active_pedo.lock();
        Game::look_at = {pedo.pos.x + PEDO_WIDTH, pedo.pos.y};
    }
}
