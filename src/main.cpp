// Copyright 2022 NNTU-CS
#include <iostream>
#include "train.h"
#include <fstream>
#include <vector>
#include <chrono>

using namespace std;
int main() {
    vector<int> train_lengths = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100,
                                 150, 200, 250, 300, 350, 400, 450, 500}; 
    int num_trials = 10;  
    vector<vector<int>> op_counts_off(train_lengths.size());
    vector<vector<int>> op_counts_on(train_lengths.size());
    vector<vector<int>> op_counts_random(train_lengths.size());
    vector<vector<long long>> time_taken_off(train_lengths.size());
    vector<vector<long long>> time_taken_on(train_lengths.size());
    vector<vector<long long>> time_taken_random(train_lengths.size());

    for (size_t i = 0; i < train_lengths.size(); ++i) {
        int n = train_lengths[i];
        for (int trial = 0; trial < num_trials; ++trial) {
            Train train_off;
            for (int j = 0; j < n; ++j) {
                train_off.addCar(false);
            }
            train_off.setAllLightsOff();
            auto start_off = chrono::high_resolution_clock::now();
            train_off.resetOpCount();
            int length_off = train_off.getLength(); 
            auto end_off = chrono::high_resolution_clock::now();
            auto duration_off = chrono::duration_cast<chrono::microseconds>(end_off - start_off);
            op_counts_off[i].push_back(train_off.getOpCount());
            time_taken_off[i].push_back(duration_off.count());
            Train train_on;
            for (int j = 0; j < n; ++j) {
                train_on.addCar(true);
            }
             train_on.setAllLightsOn();
            auto start_on = chrono::high_resolution_clock::now();
            train_on.resetOpCount();
            int length_on = train_on.getLength(); 
            auto end_on = chrono::high_resolution_clock::now();
            auto duration_on = chrono::duration_cast<chrono::microseconds>(end_on - start_on);
            op_counts_on[i].push_back(train_on.getOpCount());
            time_taken_on[i].push_back(duration_on.count());
            Train train_random;
            for (int j = 0; j < n; ++j) {
                train_random.addCar(false); 
            }
            train_random.setRandomLights();
            auto start_random = chrono::high_resolution_clock::now();
            train_random.resetOpCount();
            int length_random = train_random.getLength(); 
            auto end_random = chrono::high_resolution_clock::now();
            auto duration_random = chrono::duration_cast<chrono::microseconds>(end_random - start_random);
            op_counts_random[i].push_back(train_random.getOpCount());
            time_taken_random[i].push_back(duration_random.count());
        }
    }
    vector<double> avg_op_counts_off(train_lengths.size());
    vector<double> avg_op_counts_on(train_lengths.size());
    vector<double> avg_op_counts_random(train_lengths.size());
    vector<double> avg_time_taken_off(train_lengths.size());
    vector<double> avg_time_taken_on(train_lengths.size());
    vector<double> avg_time_taken_random(train_lengths.size());
    for (size_t i = 0; i < train_lengths.size(); ++i) {
      double sum_op_off = 0;
      double sum_op_on = 0;
      double sum_op_random = 0;
      double sum_time_off = 0;
      double sum_time_on = 0;
      double sum_time_random = 0;
      for (int trial = 0; trial < num_trials; ++trial) {
        sum_op_off += op_counts_off[i][trial];
        sum_op_on += op_counts_on[i][trial];
        sum_op_random += op_counts_random[i][trial];
        sum_time_off += time_taken_off[i][trial];
        sum_time_on += time_taken_on[i][trial];
        sum_time_random += time_taken_random[i][trial];
      }
      avg_op_counts_off[i] = sum_op_off / num_trials;
      avg_op_counts_on[i] = sum_op_on / num_trials;
      avg_op_counts_random[i] = sum_op_random / num_trials;
      avg_time_taken_off[i] = sum_time_off / num_trials;
      avg_time_taken_on[i] = sum_time_on / num_trials;
      avg_time_taken_random[i] = sum_time_random / num_trials;
    }
    ofstream op_file("op_data.txt");
    if (op_file.is_open()) {
        for (size_t i = 0; i < train_lengths.size(); ++i) {
            op_file << train_lengths[i] << " " << avg_op_counts_off[i] << " "
                    << avg_op_counts_on[i] << " " << avg_op_counts_random[i] << endl;
        }
        op_file.close();
    } else {
        cerr << "Unable to open op_data.txt" << endl;
    }

    ofstream time_file("time_data.txt");
    if (time_file.is_open()) {
        for (size_t i = 0; i < train_lengths.size(); ++i) {
            time_file << train_lengths[i] << " " << avg_time_taken_off[i] << " "
                      << avg_time_taken_on[i] << " " << avg_time_taken_random[i] << endl;
        }
        time_file.close();
    } else {
        cerr << "Unable to open time_data.txt" << endl;
    }
    string gnuplot_script = R "(
    # Plotting script for operation counts
    set terminal png size 800,600
    set output "result/plot-1.png"
    set title "Operation Counts vs. Train Length"
    set xlabel "Train Length (n)"
    set ylabel "Average Operation Count"
    plot "op_data.txt" using 1:2 with linespoints title "All Lights Off", \
         "op_data.txt" using 1:3 with linespoints title "All Lights On", \
         "op_data.txt" using 1:4 with linespoints title "Random Lights"
    # Plotting script for execution time
    set terminal png size 800,600
    set output "result/plot-2.png"
    set title "Execution Time vs. Train Length"
    set xlabel "Train Length (n)"
    set ylabel "Average Execution Time (microseconds)"
    plot "time_data.txt" using 1:2 with linespoints title "All Lights Off", \
         "time_data.txt" using 1:3 with linespoints title "All Lights On", \
         "time_data.txt" using 1:4 with linespoints title "Random Lights"
    )";
    ofstream gnuplot_commands("gnuplot_script.gp");
    gnuplot_commands << gnuplot_script << endl;
    gnuplot_commands.close();
    int result = system("gnuplot gnuplot_script.gp");
    if (result != 0) {
        cerr << "Error executing gnuplot.  Make sure gnuplot is installed and in your system's PATH." << endl;
    } else {
        cout << "Plots generated successfully in the 'result' directory." << endl;
    }
    return 0;
}


