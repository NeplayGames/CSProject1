#include <chrono>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cmath>

class stopwatch{
private:
    std::chrono::high_resolution_clock::time_point t1;
    std::chrono::high_resolution_clock::time_point t2;
    bool timing;
public:
    stopwatch() : timing(false) {
        t1 = std::chrono::high_resolution_clock::time_point::min();
        t2 = std::chrono::high_resolution_clock::time_point::min();
    }
    void start() {
        if (!timing) {
            timing = true;
            t1 = std::chrono::high_resolution_clock::now();
        }
    }
    void stop() {
        if (timing) {
            t2 = std::chrono::high_resolution_clock::now();
            timing = false;
        }
    }
    void reset() {
        t1 = std::chrono::high_resolution_clock::time_point::min();
        t2 = std::chrono::high_resolution_clock::time_point::min();
        timing = false;
    }
    double getTime() {
        std::chrono::duration<double> elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        return elapsed.count();
    }
};

int* randNumArray(const int size, const int seed) {
    srand(seed);
    int* array = new int[size];
    for (int i = 0; i < size; i++) {
        array[i] = std::rand() % 1000000;
    }
    return array;
}

void quickSort(int* array, int low, int high) {
    if (low < high) {
        int pivot = array[high];
        int i = low - 1;

        for (int j = low; j < high; j++) {
            if (array[j] <= pivot) {
                i++;
                std::swap(array[i], array[j]);
            }
        }
        std::swap(array[i + 1], array[high]);
        int pi = i + 1;

        quickSort(array, low, pi - 1);
        quickSort(array, pi + 1, high);
    }
}

int main(int argc, char** argv) {
    int* array;
    int size, seed;

    // Check the command line args
    if (argc < 3) {
        std::cerr << "usage: " << argv[0] << " [amount of random nums to generate] [seed value for rand]" << std::endl;
        exit(-1);
    }

    // Convert cstrings to ints
    {
        std::stringstream ss1(argv[1]);
        ss1 >> size;
    }
    {
        std::stringstream ss1(argv[2]);
        ss1 >> seed;
    }

    // Get the random numbers
    array = randNumArray(size, seed);

    stopwatch S1;
    S1.start();

    // Perform quick sort
    quickSort(array, 0, size - 1);

    S1.stop();
    // Print out the time
    std::cerr << "elapsed time: " << S1.getTime() << std::endl;

    // Delete the heap memory
    delete[] array;
}
