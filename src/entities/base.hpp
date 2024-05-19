#pragma once

#include <memory>
#include <vector>

class Entity {
  public:
    float x, y, width, height;

    Entity(float x, float y, float width, float height)
        : x(x), y(y), width(width), height(height) {}

    bool intersects_with(const Entity&);

    virtual void update() = 0;
    virtual void draw() = 0;
};

namespace Game {
    inline std::vector<std::shared_ptr<Entity>> entities;
};
