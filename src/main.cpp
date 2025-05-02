// Copyright 2022 NNTU-CS
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <chrono>
#include "train.h"

using namespace std;
int main() {
    ofstream ops_file("ops.csv");
    ofstream time_file("time.csv");
    if (!ops_file.is_open() || !time_file.is_open()) {
        cerr << "Error opening output files!" << endl;
        return 1;
    }
    ops_file << "Length,AllOff,AllOn,Random" << endl;
    time_file << "Length,AllOff,AllOn,Random" << endl;
    int start_length = 2;
    int end_length = 100;
    int step = 5;
    for (int length = start_length; length <= end_length; length += step) {
        cout << "Running for length: " << length << endl;
        long long ops_all_off = 0;
        long long ops_all_on = 0;
        long long ops_random = 0;
        long long time_all_off = 0;
        long long time_all_on = 0;
        long long time_random = 0;
        int num_trials = 10; 
        for (int trial = 0; trial < num_trials; ++trial) {
            Train train_all_off;
            for (int i = 0; i < length; ++i) {
                train_all_off.addCar(false);
            }
            auto start_time = chrono::high_resolution_clock::now();
            train_all_off.getLength();
            auto end_time = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
            ops_all_off += train_all_off.getOpCount();
            time_all_off += duration.count();
            Train train_all_on;
            for (int i = 0; i < length; ++i) {
                train_all_on.addCar(true);
            }
            start_time = chrono::high_resolution_clock::now();
            train_all_on.getLength();
            end_time = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
            ops_all_on += train_all_on.getOpCount();
            time_all_on += duration.count();
            Train train_random;
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> distrib(0, 1); // 0 or 1
            for (int i = 0; i < length; ++i) {
                train_random.addCar(distrib(gen));
            }
            start_time = chrono::high_resolution_clock::now();
            train_random.getLength();
            end_time = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
            ops_random += train_random.getOpCount();
            time_random += duration.count();
        }
        ops_all_off /= num_trials;
        ops_all_on /= num_trials;
        ops_random /= num_trials;
        time_all_off /= num_trials;
        time_all_on /= num_trials;
        time_random /= num_trials;
        ops_file << length << "," << ops_all_off << "," << ops_all_on << "," << ops_random << endl;
        time_file << length << "," << time_all_off << "," << time_all_on << "," << time_random << endl;
    }
    ops_file.close();
    time_file.close();
    return 0;
}


