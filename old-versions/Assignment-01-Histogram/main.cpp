/**
 * Sean McGlincy
 * Parallel Systems
 * Assignment 1
 *
 * Dependencies: I am using CLION as an IDE which uses CMAKE 3.8, and GCC, C+11
 * gcc (GCC) 4.8.5 20150623 (Red Hat 4.8.5-11)
 *
 *
 * Program:
 * The program begins by checking the terminal arguments before starting a high grade timer.
 * Five time measurements are taken and average together at the end of the program.
 * The program will read in a binary file of random intagers, positive or negative.
 * The program copies these binary values to an array where it's processed in 2N time.
 *
 * The first pass finds the min and max values.
 * An array is made with zero values.
 * The second pass increments the histogram.
 *  The program will print out to the terminal as well as create a text file after the timer has stopped.
 *
 * Running the File:
 * I recommend putting these in the same folder
 * To run file execute the binary file as ./filename
 * arg 1: is the filepath
 * arg 2: is the number of intervals
 * Example:  ./Assignment_01_Histogram random.binary 36
 *
 * */
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <limits>
#include <math.h>
#include <cmath>

using namespace std;
using namespace chrono;

int main(int argc, char *argv[]) {

    //////// USER INPUT//////////////
    int test = argc;
    if (argc != 3) {
        cout << "Error Error" << endl;
        cout << "Please provide: binary data file and interval size" << endl;
    }

    string filePath = argv[1];
    int intervalSize = atoi(argv[2]);
    cout << "Starting Program" << endl;

    //////// Start Clock //////////////
    // Use Chrono for high grade clock
    int time_samples = 5;
    high_resolution_clock::time_point clock_start[time_samples];
    high_resolution_clock::time_point clock_end[time_samples];
    for (int i = 0; i < time_samples; i++) {
        clock_start[i] = high_resolution_clock::now();
    }


    // Variables
    string line;
    int size = 0;
    vector<int> myData;
    int max = numeric_limits<int>::min(); // minimum value
    int min = numeric_limits<int>::max();


    //////// OPEN FILE //////////////
    ifstream fileInput;
    fileInput.open(filePath, ios::binary | ios::ate);

    if (fileInput.is_open()) {

        //////// Read FILE //////////////
        // Get file size
        fileInput.seekg(0, ios::end);
        int fileLength = fileInput.tellg();
        fileInput.seekg(0);

        // Resize array and copy data
        size = fileLength / sizeof(int);
        myData.resize(size);
        fileInput.read((char *) &myData[0], fileLength);
        fileInput.close();
        //////// End FILE //////////////

    } else {
        cout << "Can Not open file..." << endl;
        exit(1);
    }

    // Find max and min
    for (int i = 0; i < size; i++) {
        if (min > myData[i])
            min = myData[i];
        if (max < myData[i])
            max = myData[i];
    }


    // FIRST VERSION
//    int range = abs(max-min);
//    if (intervalSize < 1)
//        intervalSize = 1;
//    if (intervalSize > range)
//        intervalSize = range;
//
//    int arraySize = ceil( (double) range / intervalSize) ;
//
//    if(arraySize <= (max - min) / intervalSize)
//        arraySize++;
//
//    vector<int> data(  arraySize  , 0);
//    for (int i = 0; i < size; i++) {
//        data[(myData[i] - min) / intervalSize]++;
//    }


    int range = abs(max - min);
    if (intervalSize < 1)
        intervalSize = 1;
    if (intervalSize > range)
        intervalSize = range;

    int bucketSize =  range / intervalSize;
    bucketSize++;

    vector<int> data(intervalSize, 0);
    for (int i = 0; i < size; i++) {
        data[(myData[i] - min) / bucketSize]++;
    }


    //////// Print //////////////
    cout << "Sample Size: " << size << endl;
    cout << "Min Value: " << min << endl;
    cout << "Max Value: " << max << endl;
    cout << "bucket Range: " << bucketSize -1  << endl;


    for (int i = 0; i < data.size(); ++i) {
        cout << "[" << min + (i * bucketSize) << ", " << min + ((i + 1) * bucketSize) - 1 << "] : " << data[i] << endl;
    }


    ////////  END CLOCK //////////////
    //////// GET TIME //////////////
    for (int i = 0; i < time_samples; i++) {
        clock_end[i] = high_resolution_clock::now();
    }

    // Average time and convert to Micro Sec; 1 sec = 1,000,000 micro sec
    double total = 0;
    for (int i = 0; i < time_samples; i++) {
        chrono::duration<double, std::milli> diff = clock_end[i] - clock_start[i];  // Time in Micro Sec
        total += diff.count();
    }
    total = total / time_samples;

    cout << "AVG Time: " << total << " Milli Seconds" << endl;
    cout << "Program complete!" << endl;
    return 0;
}