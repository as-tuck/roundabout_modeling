#ifndef CAR_H
#define CAR_H

//Represents a car at a roundabout
struct Car{
    bool is_moving; //is the car currently in motion: used for pedestrian logic
    bool null_car; //if set to true, this car does not exist, though the space of a car remains
    int start; //beginning location, [0,4]
    int dest_relative; //destination relative to start: +1: right, +2: straight, +3: left
    int entry_t; //tick at which this car reached the front of the roundabout, just before entrance
    int exit_t; //tick at which this car exited the roundabout exit

    Car(); //creates a car with null_car set to true (everything else about it does not matter)
    Car(int location, int destination); //creates a car with dest_relative == destination
    Car(const Car& other); //defined shallow copy with modulus to enforce start/dest_relative bounds

    Car operator=(const Car& other);
};

#endif