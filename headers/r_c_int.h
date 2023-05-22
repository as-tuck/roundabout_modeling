#ifndef R_CAR_INT_H
#define R_CAR_INT_H

#include <list>
using std::list;
#include <vector>
using std::vector;
#include "car.h"

//represents the interior of a roundabout, the actual circle
struct r_c_int{
    vector<list<Car>> circle; //indexed array of cars, entry indices are even, exit indices are odd

    r_c_int(); //constructs an empty roundabout interior

    //checks each "exit list" (odd indices) to see if any car should exit
    //returns 1 value for each exit list
        //true means car in list 2*n+1 should exit
    //cars exit from position n to ((n+1)%8)/2
    vector<bool> checkToExit() const;
    //moves cars in each list to the next, accounts for cars wanting to exit
    //returns 1 value for each exit list
        //returns the cars exiting the roundabout
    vector<Car> cycleCars(vector<bool> exitstate);
    //adds a car newcar from location start
    void addCar(Car newcar, int start);

    void clear();

    bool allEmpty() const;
};

#endif