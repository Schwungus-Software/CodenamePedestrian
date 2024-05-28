#include <sstream>

#include "sounds.hpp"

void load_sounds() {
    Sounds::pedo_die = LoadSound("assets/die.wav");
    Sounds::crash = LoadSound("assets/fucked.wav");

    for (std::size_t i = 0; i < Sounds::footstep.size(); i++) {
        std::stringstream path_builder;
        path_builder << "assets/footstep-";
        path_builder << i + 1;
        path_builder << ".wav";

        Sounds::footstep[i] = LoadSound(path_builder.str().c_str());
    }
}

const Sound& Sounds::random_footstep() {
    return footstep[GetRandomValue(0, footstep.size() - 1)];
}
