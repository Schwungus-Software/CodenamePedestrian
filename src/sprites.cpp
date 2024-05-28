#include "sprites.hpp"

void load_sprites() {
    Sprites::Pedo::standing = LoadTexture("assets/pedo-stand.png");
    Sprites::Pedo::walking[0] = LoadTexture("assets/pedo-walk-1.png");
    Sprites::Pedo::walking[1] = LoadTexture("assets/pedo-walk-2.png");

    Sprites::car = LoadTexture("assets/car.png");
}
