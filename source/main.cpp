#include <iostream>
using std::cout;
#include <fstream>
using std::fstream;
#include <ctime>
#include <string>
using std::string;
#include "..\headers\roundabout.h"

void generateCars(int ncars);
void clearFiles();
void clearCarData();

//lane: 0-3
//type: entry or exit
//calculates average traffic flow in a lane (cars / tic)
void trafficFlow(int lane, string type);

//type: entry or exit
//calculates average traffic flow across all lanes
void meanTrafficFlow(string typ, int numpeds);

const int NCARS = 150;

int main() {
    clearFiles();
    int ped_count_in = 0;
    int ped_count_out = 0;
    try {
        generateCars(NCARS);
        Roundabout traffic(0);
        
        //run roundabout until all 150 cars have exited
        while(traffic.checkState()) {
            traffic.handle();
        }

        //get pedestrian counts
        ped_count_in = traffic.pedestrians_in();
        ped_count_out = traffic.pedestrians_out();

        //write to cardata.txt to get flow value for each entry,exit
        for(int j = 0; j < 4; j++) {
            trafficFlow(j,"entry");
            trafficFlow(j,"exit");
        }
        
        //write to cardata2.csv to get mean flow value across all entries,exits
        meanTrafficFlow("entry", ped_count_in);
        meanTrafficFlow("exit", ped_count_out);

        // for i < 15, pedestrians are generated too frequently for cars to make it through the roundabout.
        for(int i = 200; i >= 15; i = i / 1.45 ) {
            traffic.reset(i);
            clearCarData(); //clean files

            //run roundabout until all 150 cars have exited
            while(traffic.checkState()) {
                traffic.handle();
            }

            //get pedestrian counts
            ped_count_in = traffic.pedestrians_in();
            ped_count_out = traffic.pedestrians_out();

            //write to cardata.txt to get flow value for each entry,exit
            for(int j = 0; j < 4; j++) {
                trafficFlow(j,"entry");
                trafficFlow(j,"exit");
            }
            
            //write to cardata2.csv to get mean flow value across all entries,exits
            meanTrafficFlow("entry", ped_count_in);
            meanTrafficFlow("exit", ped_count_out);
        }
    } catch(string e) {
        cout << e << '\n';
    }
    system("pause");
}

void generateCars(int ncars) {
    fstream f;
    f.open(".\\data\\cars.txt", std::ios::out);
    if(!f.is_open()) {
        throw("cars.txt -- generate cars");
    }

    //write [ncars] random cars into a file
    int loc;
    int dest;
    string line = "";
    for(int i = 0; i < ncars; i++) {
        loc = rand() % 4;
        dest = (rand() % 3) + 1;

        line += std::to_string(loc) + ',' + std::to_string(dest) + '\n';
    }
    f << line;
    f.flush();

    f.close();
}

void clearFiles() {
    fstream f;
    f.open(".\\data\\cardata.txt", std::ios::out | std::ios::trunc);
    f.close();
    f.open(".\\data\\cardata2.csv", std::ios::out | std::ios::trunc);
    f << "car flow,pedestrian count,direction\n";
    f.close();
    f.open(".\\data\\peddata.txt", std::ios::out | std::ios::trunc);
    f << "start,dest,enter,exit\n";
    f.close();
    for(int i = 0; i < 4; i++) {
        f.open(".\\data\\entryflow" + std::to_string(i) + ".txt", std::ios::out | std::ios::trunc);
        f.close();
        f.open(".\\data\\exitflow" + std::to_string(i) + ".txt", std::ios::out | std::ios::trunc);
        f.close();
    }
}

void clearCarData() {
    //does not clear cardata2.csv
    fstream f;
    f.open(".\\data\\cardata.txt", std::ios::out | std::ios::trunc);
    f.close();
    f.open(".\\data\\peddata.txt", std::ios::out | std::ios::trunc);
    f << "start,dest,enter,exit\n";
    f.close();
    for(int i = 0; i < 4; i++) {
        f.open(".\\data\\entryflow" + std::to_string(i) + ".txt", std::ios::out | std::ios::trunc);
        f.close();
        f.open(".\\data\\exitflow" + std::to_string(i) + ".txt", std::ios::out | std::ios::trunc);
        f.close();
    }
}

void trafficFlow(int lane, string type) {
    fstream fin;
    fin.open(".\\data\\" + type + "flow" + std::to_string(lane) + ".txt", std::ios::in);
    fstream fout;
    fout.open(".\\data\\cardata.txt", std::ios::out | std::ios::app | std::ios::binary);
    if(!fin.is_open()) {
        throw("fin open, trafficflow");
    }
    if(!fout.is_open()) {
        throw("fout open, trafficFlow");
    }

    int sum = 0; // tics elapsed from first car to last car
    int init = 0; // tic when first car reached
    int num = 0; // count of cars
    string s;
    string line = "";
    while(!fin.eof()) {
        getline(fin, s);
        if(!s.empty()) {
            sum = stoi(s);
            if(num == 0) {
                init = sum;
            }
            num++;
        }
    }
    // sum is currently the number of tics from 0 to the last car, subtract first car's tic to get tics elapsed
    sum -= init;
    if(sum == 0) {
        sum = 1; // prevent any divide by 0
    }
    // cars per tic -> num/sum
    line = type + ", lane " + std::to_string(lane) + "\nnumcars: " + std::to_string(num) + "\naverage flow: " + std::to_string(float(num) / float(sum)) + "\n\n";
    fout << line;
    fout.flush();

    fin.close();
    fout.close();
}

void meanTrafficFlow(string type, int numpeds) {
    fstream fin;
    fstream fout;
    fin.open(".\\data\\cardata.txt", std::ios::in);
    fout.open(".\\data\\cardata2.csv", std::ios::out | std::ios::app);
    if(!fin.is_open()) {
        throw("fin open, meantrafficflow");
    }
    if(!fout.is_open()) {
        throw("fout open, meantrafficFlow");
    }
    string s = "";
    //number of cars through, total
    int numcars = 0;
    //decimal values of the mean, expressed as an integer
    int means_i = 0;
    //mean re-expressed as a float between 0 and 1
    float means_f = 0;
    //values for the current line
    int curnum = 0;
    int curmean = 0;

    while(!fin.eof()) {
        getline(fin, s);
        if(!s.empty()) {
            //type serves as a search key, if it is not in the line, look at the next line
            if(s.find(type) == string::npos) {
                getline(fin,s);
            } else {
                // _ denotes the first character we wish to get, beyond that we go until the end of line

                //"numcars: _"
                getline(fin,s);
                s = s.substr(9);
                curnum = stoi(s);

                //"average flow: 0._"
                getline(fin,s);
                s = s.substr(16);
                curmean = stoi(s) / 4;

                numcars += curnum;
                means_i += curmean;
            }
            //flow is bound [0,1], so if we get 0's after the decimal and any cars have gone through, then the mean flow is 1
            if(means_i == 0 && numcars != 0) {
                means_i = 1;
            }
        }
    }
    means_f = float(means_i) / 1000000;

    //carflow,numpeds
    string line = std::to_string(means_f) + ',' + std::to_string(numpeds) + ',' + type + '\n';

    fout << line;
    fout.flush();

    fin.close();
    fout.close();
}