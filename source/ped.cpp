#include "..\headers\ped.h"

Ped::Ped() {
    dest = true;
    start = 0;
    entry_t = 0;
    exit_t = 0;
}

Ped::Ped(bool destination, int location) {
    dest = destination;
    start = location;
    entry_t = 0;
    exit_t = 0;
}

Ped::Ped(bool destination, int location, int tic) {
    dest = destination;
    start = location;
    entry_t = tic;
    exit_t = tic;
}

Ped::Ped(Ped& other, int tic) {
    this->dest = other.dest;
    this->start = other.start;
    this->entry_t = tic;
    this->exit_t = tic;
}