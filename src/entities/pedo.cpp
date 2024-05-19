#include "raylib.h"

#include "entities/pedo.hpp"

void Pedo::update() {
    // nop for now.
}

void Pedo::draw() {
    DrawRectangle(x, y, width, PEDO_VISUAL_HEIGHT, RED);
}
