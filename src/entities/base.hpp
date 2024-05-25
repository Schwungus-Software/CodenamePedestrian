#pragma once

#include <memory>
#include <vector>

#include "raylib.h"

class Entity {
  public:
    Vector2 pos, vel;
    float width, height;

    bool deletion_marker = false;

    Entity(float x, float y, float width, float height)
        : pos{x, y}, vel{0.0f, 0.0f}, width(width), height(height) {}

    bool intersects_with(const Entity&);

  protected:
    void apply_velocity();

  public:
    virtual void update() = 0;
    virtual void draw() = 0;
};

namespace Game {
    inline std::vector<std::shared_ptr<Entity>> entities;
};
