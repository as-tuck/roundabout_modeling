#include "..\headers\car.h"

Car::Car() {
    null_car = true;
    is_moving = false;
    entry_t = 0;
    exit_t = 0;
    start = 0;
    dest_relative = 0;
}

Car::Car(int location,int destination) {
    null_car = false;
    is_moving = false;
    start = location % 4; //I get exactly 4 cars with the same bogus start values every time and I do not know why so we're enforcing start%4
    entry_t = 0;
    exit_t = 0;
    dest_relative = destination % 3 + 1; //no errors here like start but I'm scared
}

Car::Car(const Car& other) {
    this->null_car = other.null_car;
    this->is_moving = other.is_moving;
    this->entry_t = other.entry_t;
    this->exit_t = other.exit_t;
    this->start = other.start % 4;
    this->dest_relative = other.dest_relative % 3 + 1;
}

Car Car::operator=(const Car& other) {
    this->null_car = other.null_car;
    this->is_moving = other.is_moving;
    this->entry_t = other.entry_t;
    this->exit_t = other.exit_t;
    this->start = other.start % 4; 
    this->dest_relative = other.dest_relative % 3 + 1;

    return other;
}