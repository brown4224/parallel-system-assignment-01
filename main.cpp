/**
 * Sean McGlincy
 * Parallel Systems
 * Assignment 1
 *
 * Dependencies: I am using CLION as an IDE which uses CMAKE 3.8, and GCC, C+11
 * gcc (GCC) 4.8.5 20150623 (Red Hat 4.8.5-11)
 * Running on Cento 7
 *
 *
 * Program:
 * The program begins by checking the terminal arguments before starting a high grade timer.
 * Five time measurements are taken and average together at the end of the program.
 * The program will read in a binary file of random intagers, positive or negative.
 *
 * The program copies these binary values to an array.  It is assumed that the file can be larger then the operating system
 * and the program will read in 1,000 integers at a time.  The program will also check to see if there are less then 1,000 integers
 * when importing the data.
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
#include <assert.h>

using namespace std;
using namespace chrono;

int main(int argc, char *argv[]) {
    cout << "Starting Program" << endl;


    //////// Start Clock //////////////
    // Use Chrono for high grade clock
    int time_samples = 5;
    high_resolution_clock::time_point clock_start[time_samples];
    high_resolution_clock::time_point clock_end[time_samples];
    for (int i = 0; i < time_samples; i++) {
        clock_start[i] = high_resolution_clock::now();
    }



    //////// USER INPUT//////////////
    if (argc != 3) {
        cout << "Error Error" << endl;
        cout << "Please provide: binary data file and interval size" << endl;
        exit(1);
    }



    assert(argc == 3);
    assert(argv[2] > 0 );
    string filePath = argv[1];
    int intervalSize = atoi(argv[2]);


    //////// Variables //////////////
    int unit = sizeof(int);
    int bufferSize = unit * 1000;  // read in chuncks of file
    int size = bufferSize;
    int bucketSize = 0;
    int fileLength = 0;
    int max = numeric_limits<int>::min();
    int min = numeric_limits<int>::max();
    vector<int> buffer (bufferSize, 0);
    vector<int> data(intervalSize, 0);



    //////// OPEN FILE //////////////
    ifstream fileInput;
    fileInput.open(filePath, ios::binary);
    if (fileInput.is_open()) {

        //////// Read FILE //////////////
        // Get file size
        fileInput.seekg(0, ios::end);
        fileLength = fileInput.tellg();
        fileInput.seekg(0);

        //////// First Pass:  Min Max   //////////////
        for(int i = 0; i < fileLength; i += bufferSize){
            //  Check if buffer is less then remainder of file
            if(fileLength - i < bufferSize ) {
               size = fileLength - i;
            }

            fileInput.read((char *) &buffer[0], size);

            // Find max and min
            for (int i = 0; i < size / unit; i++) {
                if (min > buffer[i])
                    min = buffer[i];
                if (max < buffer[i])
                    max = buffer[i];
            }
        }

        // Reset
        fileInput.seekg(0);
        size = bufferSize;

        // Check Values and calculte bucket size
        int range = abs(max - min);
        if (intervalSize > range){  // if user requesting to many buckets
            intervalSize = range;
            data.resize(intervalSize);
        }
        bucketSize =  range / intervalSize;
        bucketSize++;

        //////// Second Pass:  add the values   //////////////
        for(int i = 0; i < fileLength; i += bufferSize){
            if(fileLength - i < bufferSize ) {   //  Check if buffer is less then remainder of file
                size = fileLength - i;
            }

            fileInput.read((char *) &buffer[0], size);

            // Process Data
            for (int i = 0; i < size / unit; i++) {
                data[(buffer[i] - min) / bucketSize]++;
            }
        }

        fileInput.close();
        //////// End FILE //////////////

    } else {
        cout << "Can Not open file..." << endl;
        exit(1);
    }


    //////// Print //////////////
    cout << "Sample Size: " <<  fileLength / unit << endl;
    cout << "Min Value: " << min << endl;
    cout << "Max Value: " << max << endl;
    cout << "bucket Range: " << bucketSize -1  << endl;


    for (int i = 0; i < data.size(); ++i) {
        cout << "[" << min + (i * bucketSize) << ", " << min + ((i + 1) * bucketSize) - 1 << "] : " << data[i] << endl;
    }

    // Release resources
    buffer.clear();
    data.clear();


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