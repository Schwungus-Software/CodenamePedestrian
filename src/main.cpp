#include "raylib.h"

#include "defs.hpp"

extern void load_sounds();
extern void load_sprites();

extern void restart();
extern void update();
extern void draw();

int main(int, char**) {
    InitWindow(800, 600, "Codename: Pedestrian");
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    InitAudioDevice();

    load_sprites();
    load_sounds();

    SetTargetFPS(TICK_RATE);

    restart();

    while (!WindowShouldClose()) {
        update();
        draw();
    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
