#include "..\headers\r_c_ext.h"

r_c_ext::r_c_ext() {
    exit = Car();
    entrance = Car();
    entrancelane = list<Car>();
}

void r_c_ext::addCar(Car newcar, bool exiting) {
    if(newcar.null_car) {
        return;
    }
    if(exiting) {
        exit = newcar;
    } else {
        entrancelane.push_back(newcar);
    }
}

Car r_c_ext::moveCarsEx(bool ped, int curtic) {
    Car t = Car();
    exit.exit_t = curtic;
    // !ped == no pedestrians, can move
    if(!ped && exit.is_moving) {
        t = exit;
        exit.null_car = true;
    } else if(!exit.is_moving) {
        exit.is_moving = true;
    }

    return t;
}

Car r_c_ext::moveCarsEnt(bool ped, int curtic) {
    Car t = entrance;
    // !ped == no pedestrians, can move
    if(!ped && !entrancelane.empty()) {
        entrance = entrancelane.front();
        entrancelane.pop_front();
        entrancelane.front().entry_t = curtic;
    } else {
        entrance.null_car = true;
    }
    return t;
}

void r_c_ext::clear() {
    entrance.null_car = true;
    exit.null_car = true;
    entrancelane.clear();
}

bool r_c_ext::allEmpty() const {
    if(entrance.null_car && exit.null_car && (entrancelane.empty() || entrancelane.front().null_car)) {
        return true;
    } else {
        return false;
    }
}