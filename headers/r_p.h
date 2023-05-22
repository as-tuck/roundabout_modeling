#ifndef R_PED_INT_H
#define R_PED_INT_H

#include <list>
using std::list;
#include "ped.h"

//represents a pedestrian location, either corner or island
struct r_p{
    list<Ped> left;
    list<Ped> leftx;
    list<Ped> right;
    list<Ped> rightx;

    r_p(); //constructs an empty pedestrian landing

    void addPed(Ped newped); //adds a Ped newped to left or right based on newped.dest
    void addPed(list<Ped> newped);

    //the following two methods operate the same on left/right, respectively (referred to as side below)
    //tics to cross:
        //lane width: ~15 ft
        //human walking pace: ~3mph -- 1 mile / 20 minutes -- .05 mile/minute -- 5/6000 mile/s -- 15/48000 mile/tic
        //15 ft = 0.00284091 miles
            // distance/speed = 9 tics to cross a lane
    //if car, do nothing
    //else, for all ped in side
        //if crossing, remove
        //else, set crossing to true
    //return number of pedestrians who exited
    list<Ped> handlePedLeft(bool car, int curtic);
    list<Ped> handlePedRight(bool car, int curtic);
    
    void clear();

    bool allEmpty() const;
    bool checkLeft() const;
    bool checkRight() const;

    int getNum() const;
};

#endif