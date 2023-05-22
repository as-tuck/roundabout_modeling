#ifndef R_CAR_EXT_H
#define R_CAR_EXT_H

#include <list>
using std::list;
#include "car.h"

//represents road from one of the cardinal directions of the roundabout
struct r_c_ext{
    list<Car> entrancelane; //the lane leading into the roundabout
    Car entrance; //the car right about to enter the roundabout
    Car exit; //the lane leading away from the roundabout

    r_c_ext(); //constructs an empty entrance and exit lane

    void addCar(Car newcar, bool exiting); //adds a Car 'newcar' to the entrance lane

    //from entrance:
        //returns the front Car
        //removes the front Car
        //if ped, add null car to front
        //if !ped, move cars forward
    Car moveCarsEnt(bool ped, int curtic);

    //if !ped, return the car from exit and set it to a null car
    Car moveCarsEx(bool ped, int curtic);

    void clear();
    
    bool allEmpty() const;
};

#endif