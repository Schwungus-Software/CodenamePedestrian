#pragma once

#include "entities/base.hpp"

class Pedo : public Entity {
  public:
    static constexpr const float WIDTH = 4.0f, HEIGHT = 1.5f, VISUAL_HEIGHT = 9.0f,
                                 AIR_RESISTANCE = 0.92f;

    bool dying = false, safe = false;
    float die_countdown = 0.0f, footstep_countdown = 0.0f;

    Pedo(float x, float y) : Entity(x - WIDTH * 0.5f, y, WIDTH, HEIGHT) {}

    void update() override;
    void draw() override;

    bool is_active();
};

namespace Game {
    inline std::weak_ptr<Pedo> active_pedo;
    inline bool all_pedos_gone;
} // namespace Game
