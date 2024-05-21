#pragma once

#include "entities/base.hpp"

class Pedo : public Entity {
  public:
    static constexpr const float WIDTH = 4.0f, VISUAL_HEIGHT = 9.0f;

    Pedo(float x, float y) : Entity(x - WIDTH * 0.5f, y, WIDTH, 1.0) {}

    void draw() override;
    void update() override;

    bool is_active();
};

namespace Game {
    inline std::weak_ptr<Pedo> active_pedo;
}
