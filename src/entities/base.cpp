#include "entities/base.hpp"

bool Entity::intersects_with(const Entity& another) {
    return x + width >= another.x && x <= another.x + another.width && y + height >= another.y &&
           y <= another.y + another.height;
}
