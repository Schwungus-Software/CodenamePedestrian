#pragma once

#include <cstdint>
#include <optional>

#include "entities/base.hpp"

class Car : public Entity {
  public:
    enum Direction {
        FORWARD,
        BACKWARD,
    } direction;

  private:
    bool braking = false;
    float starting_velocity;
    float realization_countdown = 0.0f;

  public:
    Car(float, float, float, Direction);

    std::optional<std::size_t> get_lane_idx() const;

    void update() override;
    void draw() override;
};
