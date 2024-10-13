#include <chrono>
#include <omp.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <math.h>
#include <assert.h>

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
void merge(int *array, int beginning, int middle, int end)
{

//#pragma omp critical
//	std::cout << "beginning: " << beginning << ", middle: " << middle << ", end: " << end << "\n";

	int left_array_size = middle - (beginning - 1);
	int right_array_size = end - middle;

	int left_array[middle - (beginning - 1)];
	int right_array[end - middle];

//#pragma omp critical
	for(int i = 0; i <= left_array_size - 1; i++)
	{

		left_array[i] = array[beginning + i];

	}//end for i

//#pragma omp critical
	for(int i = 0; i <= right_array_size - 1; i ++)
	{

		right_array[i] = array[middle + 1 + i];

	}//end for i


	int i = 0;// left array index	
	int j = 0;// right array index

	int k;// main array index

	for(k = beginning; k <= end; k ++)
	{

		if(i <= left_array_size - 1 && j <= right_array_size - 1)
		{

			if(left_array[i] < right_array[j])
			{

//#pragma omp critical
				array[k] =  left_array[i];

				i++;

			}// end if
			else if(left_array[i] >= right_array[j])
			{
//#pragma omp critical
				array[k] = right_array[j];

				j++;

			}// end else

		}//end if
		else if(i <= left_array_size - 1)
		{
//#pragma omp critical
			array[k] =  left_array[i];
			i ++;

		}// end else if
		else if(j <= right_array_size - 1)
		{
//#pragma omp critical
			array[k] = right_array[j];
			j++;

		}// end else if
		else{

		}// end else


	}// end for k


}//end function


void mergesort(int * array, int beginning, int end)
{



//#pragma omp critical
	//std::cout << "thread: " << omp_get_thread_num() << "\n";


	//std::cout << "thread: " << omp_get_thread_num() << "\n";

	if(beginning < end)
	{

		int middle = (beginning + end) / 2;

//#pragma omp parallel
		{

#pragma omp task			
		mergesort(array, beginning, middle);

#pragma omp task
		mergesort(array, middle + 1, end);
		}//end parallel
	       	
#pragma omp critical
merge(array, beginning, middle, end);

	}//end if


}//end function



// seed: seeds the random number generator
int * randNumArray( const int size, const int seed ) {
	srand( seed );
	int * array = new int[ size ];
	for( int i = 0; i < size; i ++ ) {
		array[i] = std::rand() % 1000000;
	}
	return array; }




void unit_test_sort() 
{
	for(int i = 10; i <= 100; i ++)
	{

		//std::cout << "i: " << i << "\n";

		for(int j = 0; j <= 1000; j ++)
		{
			//std::cout << j << ", ";
			int * array = randNumArray( i, i );

			int array2[i];// = new int[i];

			for(int k = 0; k <= i - 1; k ++)
			{

				array2[k] = array[k];

			}//end for k

			mergesort(array, 0, i - 1);

			for(int k = 0; k <= i - 1; k ++)
			{

				bool number_in_sorted_array = false;

				for(int z = 0; z <= i - 1; z ++)
				{

					if(array2[k] == array[z])
					{

						number_in_sorted_array = true;

					}//end if


				}//end for z

				assert(number_in_sorted_array == true);

			}//end for k


			for(int k = 1; k <= i - 1; k ++)
			{
				//array[k] = 0;
				assert(array[k] > array[k - 1]);

				//assert(array[k] != array[k - 1]);
						//	std::cout << "array[" << k << "] = " << array[k] << " > " << array[k - 1] << "\n"; 

				//				std::cout << "\n";

			}//end for k

		}//end for j




	}//end for i

}//end function

	int main( int argc, char** argv ) {
		int * array; // the poitner to the array of rands
		int size, seed; // values for the size of the array
		// and the seed for generating
		// random numbers
		// check the command line args
		if( argc < 3 ){
			std::cerr << "usage: "
				<< argv[0]
				<< " [amount of random nums to generate] [seed value for rand]" << std::endl;
			exit( -1 ); }
		// convert cstrings to ints
		{
			std::stringstream ss1( argv[1] );
			ss1 >> size;
		} {
			std::stringstream ss1( argv[2] );
			ss1 >> seed; }
		// get the random numbers
		array = randNumArray( size, seed );
		// **************************
		// **************************
		// **************************
		//
		//  YOUR CODE HERE !!!
		//
		// **************************
		// **************************
		// **************************


//unit_test_sort();

		//void mergesort(int * array, int beginning, int end)


//unit_test_sort();
    stopwatch S1;
    S1.start();
omp_set_num_threads(10);

#pragma omp parallel
		mergesort(array, 0, size - 1);

		//std::cout << "merge sorted array\n";

		//for(int i = 0; i <= size - 1; i ++)
		//{

			//std::cout << array[i] << ", ";

		//}//end for i

		//std::cout << "\n";
        S1.stop();
		std::cerr << "elapsed time: " << S1.getTime() << std::endl;
		// delete the heap memory
		delete [] array;
	}//end main
