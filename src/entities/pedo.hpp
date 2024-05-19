#pragma once

#include "defs.hpp"

#include "entities/base.hpp"

class Pedo : public Entity {
  public:
    Pedo(float x, float y) : Entity(x - PEDO_WIDTH * 0.5f, y, PEDO_WIDTH, 1.0) {}

    void draw() override;
    void update() override;

    bool is_active();
};

namespace Game {
    inline std::weak_ptr<Pedo> active_pedo;
}
