// Sean McGlincy
// Parallel System
// Random number generator

#include <iostream>
#include <fstream>
using namespace std;

int main() {
    int sampleSize = 100000000;

    // Sample changes dynamically
    // Uses different seed then data generation
    srand(time(NULL));
    int maxNum = rand() % sampleSize;
//    int maxNum = 2000000;
    int lowerBound = -maxNum /2;
    string filename = "./random.binary";
    cout << "Please Wait: Creating File" << endl;

    // Create File
    ofstream fileOut (filename, ios::binary);
    srand(time(NULL));
    for(int i = 0; i< sampleSize; i++){
        int data = rand() % maxNum + lowerBound;
        fileOut.write((char*)&data, sizeof(data));

    }
    fileOut.close();
    cout << "Creating File : " <<  filename << endl;
    return 0;
}