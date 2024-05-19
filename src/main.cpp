#include "raylib.h"

#include "defs.hpp"

extern void restart();
extern void update();
extern void draw();

int main(int, char**) {
    InitWindow(800, 600, "Codename: Pedestrian");
    InitAudioDevice();

    SetTargetFPS(FRAMERATE);

    restart();

    while (!WindowShouldClose()) {
        update();
        draw();
    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
