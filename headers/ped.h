#ifndef PED_H
#define PED_H

//Represents a pedestrian at a roundabout
struct Ped{
    bool dest; //pedestrians move left(false) or right(true)
    int start; //beginning location, [0,4]
    int entry_t; //time pedestrian began crossing
    int exit_t; //time pedestrian finished crossing (reached destination)

    Ped(); //creates a pedestrian with dest == true
    Ped(bool destination, int location); //creates a pedestrian with dest == destination
    Ped(bool destination, int location, int tic);
    Ped(Ped& other, int tic);
};

#endif