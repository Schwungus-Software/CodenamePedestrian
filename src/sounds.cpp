#include "sounds.hpp"

void load_sounds() {
    Sounds::pedo_die = LoadSound("assets/die.wav");
    Sounds::crash = LoadSound("assets/fucked.wav");

    Sounds::pain[0] = LoadSound("assets/pain-1.wav");
    Sounds::pain[1] = LoadSound("assets/pain-2.wav");
    Sounds::pain[2] = LoadSound("assets/pain-3.wav");

    Sounds::footstep[0] = LoadSound("assets/footstep-1.wav");
    Sounds::footstep[1] = LoadSound("assets/footstep-2.wav");
    Sounds::footstep[2] = LoadSound("assets/footstep-3.wav");
    Sounds::footstep[3] = LoadSound("assets/footstep-4.wav");
    Sounds::footstep[4] = LoadSound("assets/footstep-5.wav");
    Sounds::footstep[5] = LoadSound("assets/footstep-6.wav");
}

const Sound& Sounds::random_footstep() {
    return footstep[GetRandomValue(0, footstep.size() - 1)];
}

const Sound& Sounds::random_pain() {
    return pain[GetRandomValue(0, pain.size() - 1)];
}
