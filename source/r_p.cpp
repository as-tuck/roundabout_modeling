#include "..\headers\r_p.h"

r_p::r_p() {
    left = list<Ped>();
    right = list<Ped>();
    leftx = list<Ped>();
    rightx = list<Ped>();
}

void r_p::addPed(Ped newped) {
    if(newped.dest) {
        right.push_back(newped);
    } else {
        left.push_back(newped);
    }
}

void r_p::addPed(list<Ped> newped) {
    while(!newped.empty()) {
        addPed(newped.front());
        newped.pop_front();
    }
}

list<Ped> r_p::handlePedLeft(bool car, int curtic) {
    list<Ped> t = list<Ped>();
    int n = 0;
    //do not start crossing if there is a car and no peds are currently crossing
    if(!car || !leftx.empty()) {
        while(!left.empty() && n < 5) {
            leftx.push_back(left.front());
            left.pop_front();
            leftx.back().exit_t = curtic + 9;
            n++;
        }
    }
    //finish crossing if in the road
    while(!leftx.empty() && curtic > leftx.front().exit_t) {
        leftx.front().exit_t = curtic;
        t.push_back(leftx.front());
        leftx.pop_front();
    }
    return t;
}

list<Ped> r_p::handlePedRight(bool car, int curtic) {
    list<Ped> t = list<Ped>();
    int n = 0;
    if(!car || !rightx.empty()) {
        while(!right.empty() && n < 5) {
            rightx.push_back(right.front());
            right.pop_front();
            rightx.back().exit_t = curtic + 9;
            n++;
        }
    }
    while(!rightx.empty() && curtic > rightx.front().exit_t) {
        rightx.front().exit_t = curtic;
        t.push_back(leftx.front());
        rightx.pop_front();
    }
    return t;
}

void r_p::clear() {
    left.clear();
    leftx.clear();
    right.clear();
    rightx.clear();
}

bool r_p::allEmpty() const {
    if(right.empty() && rightx.empty() && left.empty() && leftx.empty()) {
        return true;
    }
    else {
        return false;
    }
}

bool r_p::checkLeft() const {
    //return true if there are pestrians in or at the crossing
    return !left.empty() || !leftx.empty();
}

bool r_p::checkRight() const {
    //return true if there are pedestrians in or at the crossing
    return !right.empty() || !rightx.empty();
}

int r_p::getNum() const {
    return left.size() + right.size();
}