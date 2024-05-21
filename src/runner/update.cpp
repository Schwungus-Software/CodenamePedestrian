#include <memory>

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
}
