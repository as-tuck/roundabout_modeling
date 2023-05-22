#include "..\headers\r_c_int.h"

r_c_int::r_c_int() {
    for(int i = 0; i < 8; i++) {
        circle.push_back(list<Car>());
    }
}

vector<bool> r_c_int::checkToExit() const {
    vector<bool> t = vector<bool>();
    for(int i = 0; i < 4; i++) {
        //circle[7] -- 0, c[1] -- 1, c[3] -- 2, c[5] -- 3
        if(!circle[2*i-1].empty()) {
            t.push_back((circle[(8 + (2*i - 1)) % 8].front().start + circle[(8 + (2*i - 1)) % 8].front().dest_relative) % 4 == i);
        } else{
            t.push_back(false);
        }
    }
    return t;
}

vector<Car> r_c_int::cycleCars(vector<bool> exitstate) {
    for(int i = 0; i < 8; i++) {
        circle[i].front().is_moving = true;
        if(circle[i].front().null_car) {
            circle[i].pop_front();
        }
    }
    vector<Car> t = vector<Car>();
    vector<bool> carstate = checkToExit(); //does a car want to exit?
    for(int i = 0; i < 4; i++) { //handle exits
        //circle[7] -- 0, c[1] -- 1, c[3] -- 2, c[5] -- 3
        if(exitstate[i] && carstate[i]) { //exit path clear, car wants to exit
            circle[(8 + (2*i - 1)) % 8].front().is_moving = false;
            t.push_back(circle[(8 + (2*i - 1)) % 8].front());
            circle[(8 + (2*i - 1)) % 8].pop_front();
        } else if(!exitstate[i] && carstate[i]) { //car wants to exit, exit path is not clear
            t.push_back(Car());
            circle[(8 + (2*i - 1)) % 8].front().is_moving = false; //car cannot move
        } else { //car does not want to exit
            t.push_back(Car());
            circle[(8 + (2*i - 1)) % 8].front().is_moving = true; //car can move
        }
    }

    for(int i = 0; i < 8; i++) {
        //can the car move? has the car moved?
        if(!circle[i].empty() && circle[i].front().is_moving && (circle[(i + 1) % 8].empty() || circle[(i + 1) % 8].front().is_moving)) {
            circle[i].front().is_moving = false; //this car has moved this tic
            circle[(i+1) % 8].push_back(circle[i].front());
            circle[i].pop_front();
        }
    }

    return t;
}

void r_c_int::addCar(Car newcar, int start) {
    if(!newcar.null_car) {
        circle[2*start].push_back(newcar);
    }
}

void r_c_int::clear() {
    for(int i = 0; i < 8; i++) {
        circle[i].clear();
    }
}

bool r_c_int::allEmpty() const {
    for(int i = 0; i < 8; i++) {
        if(!circle[i].empty() && !circle[i].front().null_car) {
            return false;
        }
    }
    return true;
}