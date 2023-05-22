#include "..\headers\roundabout.h"

#include <iostream>
using std::cout;

Roundabout::Roundabout() {
    interior = r_c_int();

    curtic = 0;
    ped_dense = 500;

    ped_waves = 0;

    in_cars = 0;
    out_cars = 0;
    in_peds = 0;
    out_peds = 0;

    //these two help ensure that we never end up only adding pedestrians to one corner, which stops the system entirely
    engine.seed(0);
    generator = uniform_int_distribution<int>(0,7);

    for(int i = 0; i < 4; i++) {
        exterior.push_back(r_c_ext());
        corner.push_back(r_p());
        island.push_back(r_p());
    }
    addCars();
}

Roundabout::Roundabout(int density) {
    interior = r_c_int();

    curtic = 0;
    ped_dense = density;
    
    ped_waves = 0;

    in_cars = 0;
    out_cars = 0;
    out_peds = 0;

    //engine and generator prevent the system from getting stuck by evenly distributing pedestrians
    engine.seed(0);
    generator = uniform_int_distribution<int>(0,3);

    for(int i = 0; i < 4; i++) {
        exterior.push_back(r_c_ext());
        corner.push_back(r_p());
        island.push_back(r_p());
    }
    addCars();
}

void Roundabout::reset(int density) {
    //reset cars and pedestrians
    clearAll();
    addCars();

    //0 everything
    in_cars = 0;
    out_cars = 0;
    in_peds = 0;
    out_peds = 0;
    curtic = 0;
    ped_waves = 0;
    ped_dense = density;

    //reseed random to make results consistent
    engine.seed(0);
}

void Roundabout::addCars() {
    //read a file
    fstream f;
    f.open(".\\data\\cars.txt", std::ios::in);
    if(!f.is_open()) {
        throw("cars.txt -- roundabout");
    }

    //
    string s;
    int loc = 0;
    int dest = 0;

    //line structure of cars.txt
    //[start],[dest_relative]
    while(!f.eof()) {
        getline(f, s);
        if(!s.empty()) {
            loc = std::stoi(s.substr(0, ','));
            dest = std::stoi(s.substr(s.find(',') + 1));

            exterior[loc].addCar(Car(loc,dest), false);
        }
    }
    f.close();
}

bool Roundabout::extEmpty() const {
    //check every lane
    for(int i = 0; i < 4; i++) {
        if(!exterior[i].allEmpty()) {
            return false;
        }
    }
    return true;
}

bool Roundabout::cornEmpty() const {
    //check every corner
    for(int i = 0; i < 4; i++) {
        if(!corner[i].allEmpty()) {
            return false;
        }
    }
    return true;
}

bool Roundabout::islEmpty() const {
    //check every island
    for(int i = 0; i < 4; i++) {
        if(!island[i].allEmpty()) {
            return false;
        }
    }
    return true;
}

vector<bool> Roundabout::checkExits() const {
    //check the exits
    //returns a vector of true/false for each exit
    vector<bool> t = vector<bool>();
    for(int i = 0; i < 4; i++) {
        t.push_back(exterior[i].exit.null_car);
    }
    return t;
}

void Roundabout::countCars() {
    //check for a car in each roundabout section
    int s = 0;
    for(int i = 0; i < 8; i++) {
        if(!interior.circle[i].empty()) {
            s++;
        }
    }
    in_cars = s;
}

void Roundabout::clearAll() {
    //clear out all cars and pedestrians
    interior.clear();
    for(int i = 0; i < 4; i++) {
        exterior[i].clear();
        corner[i].clear();
        island[i].clear();
    }
}

void Roundabout::writeCars(Car data, int exit) {
    //don't write bogus fake cars
    if(data.null_car) {
        return;
    }

    out_cars++;

    //check start/dest bounds
    if(data.start >= 4 || data.start < 0) {
        throw("bad start. car " + std::to_string(out_cars) + ", start " + std::to_string(data.start) + ", dest " + std::to_string(data.dest_relative) + ", is null " + std::to_string(data.null_car));
    }
    if(data.dest_relative >= 4 || data.dest_relative < 1) {
        throw("bad dest. car " + std::to_string(out_cars) + ", start " + std::to_string(data.start) + ", dest " + std::to_string(data.dest_relative) + ", is null " + std::to_string(data.null_car));
    }

    fstream f;

    //write entry time
    f.open(".\\data\\entryflow" + std::to_string(data.start) + ".txt", std::ios::out | std::ios::app);
    if(!f.is_open()) {
        throw("entryflow" + std::to_string(data.start) + ".txt -- write cars");
    }
    f << std::to_string(data.entry_t) + '\n';
    f.flush();
    f.close();

    //write exit time
    f.open(".\\data\\exitflow" + std::to_string((data.start + data.dest_relative) % 4) + ".txt", std::ios::out | std::ios::app);
    if(!f.is_open()) {
        throw("exitflow" + std::to_string((data.start + data.dest_relative) % 4) + ".txt -- write cars");
    }
    f << std::to_string(data.exit_t) + '\n';
    f.flush();
    f.close();
}

void Roundabout::writePeds(list<Ped>& data) {
    out_peds += data.size();

    fstream f;
    f.open(".\\data\\peddata.txt", std::ios::out | std::ios::app);
    if(!f.is_open()) {
        throw("peddata.txt -- write peds");
    }

    //write details of the pedestrian: start, dest, and entry/exit times
    string line = "";
    while(!data.empty()) {
        line += std::to_string(data.front().start) + ',';
        line += std::to_string(data.front().dest) + ',';
        line += std::to_string(data.front().entry_t) + ',';
        line += std::to_string(data.front().exit_t) + '\n';

        data.pop_front();
    }
    f << line;
    f.flush();
    f.close();
}

void Roundabout::handle() {    
    //check to add pedestrians
    if(ped_dense != 0 && (curtic % ped_dense) == 0) {
        in_peds += 5;
        int index;
        Ped toAdd;
        for(int i = 0; i < 5; i++) {
            index = generator(engine);
            toAdd = p_templates[index];

            corner[toAdd.start].addPed(Ped(toAdd, curtic));
            ped_waves++;
        }
    }

    vector<bool> exiting_cars = checkExits();

    //check to move pedestrians
    list<Ped> leaving;
    list<Ped> temp;
    // directions facing roundabout
    for(int i = 0; i < 4; i++) {
        // right side (island leaving right, corner leaving left)
        // car entrance
            //pedestrians leaving system
        temp = island[i].handlePedRight(!exterior[i].entrancelane.empty() && exterior[i].entrancelane.front().is_moving, curtic);
        while(!temp.empty()) {
            leaving.push_back(temp.front());
            temp.pop_front();
        }
            //pedestrians entering system
        temp = corner[i].handlePedLeft(!exterior[i].entrancelane.empty() && exterior[i].entrancelane.front().is_moving, curtic);
        island[i].addPed(temp);

        // left side (island leaving left, corner leaving right)
        // pedestrians will also wait for cars leaving the roundabout
        // car exit
            //pedestrians leaving
        temp = island[i].handlePedLeft(exterior[i].exit.null_car && exterior[i].exit.is_moving && !exiting_cars[i], curtic);
        while(!temp.empty()) {
            leaving.push_back(temp.front());
            temp.pop_front();
        }
            //pedestrians entering
        temp = corner[i].handlePedRight(exterior[i].exit.null_car && exterior[i].exit.is_moving && !exiting_cars[i], curtic);
        island[(i + 1) % 4].addPed(temp);

        temp.clear();
    }
    writePeds(leaving);

    //move cars
        //move cars inside circle, retrieve cars exiting circle
    vector<Car> t(interior.cycleCars(exiting_cars));
    for(int i = 0; i < 4; i++) {
        //move cars out of circle
        exterior[i].addCar(t[i], true);

        //cars entering circle
        if(interior.circle[2*i].empty() || exterior[i].entrance.null_car) {
            // r_p.checkSide() == true, pedestrians at, car cannot move
            interior.addCar(exterior[i].moveCarsEnt(corner[i].checkLeft() || island[(i + 3) % 4].checkRight(), curtic), i);
        }
        //cars exiting system
        writeCars(exterior[i].moveCarsEx(island[i].checkLeft() || corner[(i + 3) % 4].checkRight(), curtic), i);
    }
    
    //increment tic
    curtic++;
}

bool Roundabout::checkState() const {
    if(interior.allEmpty() && extEmpty()) {
        return false;
    } else {
        return true;
    }
}

int Roundabout::pedestrians_out() const {
    return out_peds;
}

int Roundabout::pedestrians_in() const {
    return in_peds;
}