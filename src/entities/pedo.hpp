#pragma once

#include "entities/base.hpp"

class Pedo : public Entity {
  public:
    bool dying = false, safe = false;
    float die_countdown = 0.0f, footstep_countdown = 0.0f;

    Pedo(float, float);

    void update() override;
    void draw() override;

  private:
    int footstep_sprite = 0;
    bool flip_sprite = false;

    void draw_impl(Color);

  public:
    bool is_active();
};

namespace Game {
    inline std::weak_ptr<Pedo> active_pedo;
    inline bool all_pedos_gone;
} // namespace Game
