#pragma once

#include <optional>

#include "raylib.h"

#include "entities/base.hpp"

class Car : public Entity {
  public:
    static constexpr const float SLOWDOWN = 1024.0f;

    enum Direction {
        FORWARD,
        BACKWARD,
    } direction;

  private:
    bool braking = false;
    float despawn_countdown = 0.0f;

  public:
    Car(float left, float width, float height, float velocity, Direction direction)
        : Entity(left, 0.0f, width, height), direction(direction) {
        vel.y = velocity * (direction == Direction::FORWARD ? 1.0f : -1.0f);

        if (direction == Direction::FORWARD) {
            pos.y = -height + 1.0f;
        } else {
            pos.y = GetScreenHeight() - 2.0f;
        }
    }

    std::optional<std::size_t> get_lane_idx() const;

    void update() override;
    void draw() override;
};
