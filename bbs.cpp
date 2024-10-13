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

void bubbleSort(std::vector<int>& bucket) {
    for (size_t i = 0; i < bucket.size() - 1; i++) {
        for (size_t j = 0; j < bucket.size() - i - 1; j++) {
            if (bucket[j] > bucket[j + 1]) {
                std::swap(bucket[j], bucket[j + 1]);
            }
        }
    }
}

void bucketSort(int* array, int size) {
    const int numBuckets = 10;
    std::vector<int> buckets[numBuckets];

    // Determine the range of the elements
    int maxValue = *std::max_element(array, array + size);
    int minValue = *std::min_element(array, array + size);
    int range = maxValue - minValue + 1;

    //std::cout << "Max value: " << maxValue << "\n";
    //std::cout << "Min value: " << minValue << "\n";

    // Distribute the elements into buckets
    for (int i = 0; i < size; i++) {
        int bucketIndex = std::floor((array[i] - minValue) * numBuckets / range);
        if (bucketIndex >= numBuckets) bucketIndex = numBuckets - 1; // Edge case where value falls exactly on max
        //std::cout << "Element " << array[i] << " goes to bucket " << bucketIndex << "\n";
        buckets[bucketIndex].push_back(array[i]);
    }

    // Print the bucket sizes
    //for (int i = 0; i < numBuckets; i++) {
        //std::cout << "Bucket " << i << " size: " << buckets[i].size() << "\n";
    //}

    // Sort each bucket and concatenate the result
    int index = 0;
    for (int i = 0; i < numBuckets; i++) {
        if (!buckets[i].empty()) {
            //std::cout << "Sorting bucket " << i << "\n";
            bubbleSort(buckets[i]);
            for (int j : buckets[i]) {
                array[index++] = j;
            }
        }
    }
}

int main(int argc, char** argv) {
    int* array;
    int size, seed;

    // Check the command line args
    if (argc < 3) {
        //std::cerr << "usage: " << argv[0] << " [amount of random nums to generate] [seed value for rand]" << std::endl;
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
    //std::cout << "Generated array:\n";
    //for (int i = 0; i < size; i++) {
        //std::cout << array[i] << " ";
    //}
    //std::cout << "\n";

    stopwatch S1;
    S1.start();

    // Perform bucket sort
    bucketSort(array, size);

    S1.stop();
    // Print out the time
    std::cerr << "elapsed time: " << S1.getTime() << std::endl;

    // Print the sorted array
    //std::cout << "Sorted array:\n";
    //for (int i = 0; i < size; i++) {
      //  std::cout << array[i] << " ";
    //}
    std::cout << "\n";

    // Delete the heap memory
    delete[] array;
}
