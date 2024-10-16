
#include <chrono>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <omp.h>
#include<new>
using namespace std;
// a class to time things using the high resolution timer class
// to use,  call start() before the code you want to time.  Call
// stop() after the code you want to time.  call getTime() to return
// the elapsed time in seconds.
// to reuse the instance,  call reset(),  then time something else
class stopwatch{
private:
        std::chrono::high_resolution_clock::time_point t1;
        std::chrono::high_resolution_clock::time_point t2;
        bool timing;
public:
        stopwatch( ): timing( false ) {
                t1 = std::chrono::high_resolution_clock::time_point::min();
                t2 = std::chrono::high_resolution_clock::time_point::min();
        }
        void start( ) {
                if( !timing ) {
                        timing = true;
                        t1 = std::chrono::high_resolution_clock::now();
                }
        }
        void stop( ) {
                if( timing ) {
                        t2 = std::chrono::high_resolution_clock::now();
                        timing = false;
                }
        }
        void reset( ) {
                t1 = std::chrono::high_resolution_clock::time_point::min();
                t2 = std::chrono::high_resolution_clock::time_point::min();;
                timing = false;
        }
        // will return the elapsed time in seconds as a double
        double getTime( ) {
                std::chrono::duration<double> elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(t2-t1);
                return elapsed.count();
	}
};



// create an array of length size of random numbers
// returns a pointer to the array
// seed: seeds the random number generator
int * randNumArray( const int size, const int seed ) {

	srand( seed );
	int * array = new int[ size ];
	for( int i = 0; i < size; i ++ ) {
		array[i] = std::rand() % 1000000;
	}
	return array;
}

void customMerge(int arr[], int first, int middle, int last) {
    int* tempArray = new int[last - first + 1];
    int leftIndex = first;
    int rightIndex = middle + 1;
    int mergedIndex = 0;

    while (leftIndex <= middle && rightIndex <= last) {
        if (arr[leftIndex] < arr[rightIndex]) {
            tempArray[mergedIndex] = arr[leftIndex];
            leftIndex++;
        } else {
            tempArray[mergedIndex] = arr[rightIndex];
            rightIndex++;
        }
        mergedIndex++;
    }

    while (leftIndex <= middle) {
        tempArray[mergedIndex] = arr[leftIndex];
        leftIndex++;
        mergedIndex++;
    }
    while (rightIndex <= last) {
        tempArray[mergedIndex] = arr[rightIndex];
        rightIndex++;
        mergedIndex++;
    }

    for (int i = 0; i < mergedIndex; i++) {
        arr[first + i] = tempArray[i];
    }

    delete[] tempArray;
}


void sequential_mergesort(int* array, int first, int last) {
    if (first < last) {
        int mid = (first + last) / 2;
        sequential_mergesort(array, first, mid);
        sequential_mergesort(array, mid + 1, last);
        customMerge(array, first, mid, last); // Merging the two halves
    }
}


int main( int argc, char** argv ) {

	int * array;  // the poitner to the array of rands
	int size, seed, numThreads; // values for the size of the array
			// and the seed for generating
			// random numbers

			// check the command line args
	if( argc < 4 ){
		std::cerr << "usage: " 
			<< argv[0] 
			<< " [amount of random nums to generate] [number of threads to use] [seed value for rand]" 
			<< std::endl;
		exit( -1 );
	}

	// convert cstrings to ints
	{
		std::stringstream ss1( argv[1] );
		ss1 >> size;
	}
	{
		std::stringstream ss1( argv[2] );
		ss1 >> numThreads;
	}
	{
		std::stringstream ss1( argv[3] );
		ss1 >> seed;
	}

	// get the random numbers
	array = randNumArray( size, seed );
	// tell omp how many threads to use
        omp_set_num_threads( numThreads );

	stopwatch S1;
	S1.start();

	sequential_mergesort( array, 0, size-1);

	S1.stop();
	// print out the time
	std::cerr << "elapsed time: " << S1.getTime( ) << std::endl;

	// delete the heap memory
	delete [] array;

}