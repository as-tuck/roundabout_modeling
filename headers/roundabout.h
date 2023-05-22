#ifndef ROUNDABOUT_H
#define ROUNDABOUT_H

#include <vector>
using std::vector;
#include <fstream>
using std::fstream;
#include <string>
using std::string;
#include <random>
using std::default_random_engine;
using std::uniform_int_distribution;
#include "r_c_ext.h"
#include "r_c_int.h"
#include "r_p.h"
#include "r_p.h"

const Ped p_templates[] = {Ped(0,0),Ped(1,0),Ped(0,1),Ped(1,1),Ped(0,2),Ped(1,2),Ped(0,3),Ped(1,3)};

// represents the entire roundabout junction
class Roundabout{
private:
    r_c_int interior;
    vector<r_c_ext> exterior;

    vector<r_p> corner;
    vector<r_p> island;


    default_random_engine engine;
    uniform_int_distribution<int> generator;

    //count of:
    int in_cars; //: cars that have entered (gone past the first crosswalk)
    int out_cars; //: cars that have exited
    int in_peds; //: pedestrians that have entered the system, incremented before they cross the first lane
    int out_peds; //: pedestrians that have exited the system, incremented after they finish crossing

    int curtic; //approximately 3/8 seconds
    int ped_dense; //tic interval between adding 30 pedestrians
    int ped_waves; //how many times we have added pedestrians

    void addCars(); //reads cars.txt
    
    void writeCars(Car data, int exit); //writes to cardata.txt
    void writePeds(list<Ped>& data); //writes to peddata.txt

    bool extEmpty() const; //returns true if every element in exterior is empty
    bool cornEmpty() const; //returns true if every element in corner is empty
    bool islEmpty() const; //returns true if every element in island is empty
    
    vector<bool> checkExits() const; //check each exit to see if it is currently occupied
    void countCars(); //count the cars inside the roundabout circle
    
    void clearAll(); //clear every pedestrian and car from the system

public:
    Roundabout(); //constructs a roundabout, calls addCars/addPeds
    Roundabout(int density); //construct a roundabout with a ped_dense of density

    void reset(int density); //resets the roundabout with a new density

    //simulates a single tick of the roundabout, then increments curtic
    void handle();

    bool checkState() const; //returns false if no cars/peds left

    int pedestrians_in() const; //returns the number of pedestrians that have arrived at the roundabout
    int pedestrians_out() const; //returns the number of pedestrians that have left the roundabout
};

#endif