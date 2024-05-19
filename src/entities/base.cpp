#include "entities/base.hpp"

#include "raymath.h"

bool Entity::intersects_with(const Entity& another) {
    return pos.x + width >= another.pos.x && pos.x <= another.pos.x + another.width &&
           pos.y + height >= another.pos.y && pos.y <= another.pos.y + another.height;
}

void Entity::apply_velocity() {
    pos = Vector2Add(pos, vel);
}
