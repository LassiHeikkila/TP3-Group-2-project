// Use threadpool library from: http://threadpool.sourceforge.net/

// Use global data structure that all threads can access.
// 	State extern input_data input in header file, and
// 	input_data input in main().

// Pass only starting column integer from threader to FDM.

// Run threader via intermediate function, once each for red/black.

// Maybe have non-threaded intermediate function too, using plain
// SOR instead of checkerboard in the case cores = 1.

#include "headers.h"

input_args* input;

int main(int argc, char const *argv[])
{
	int core_count = std::thread::hardware_concurrency();
	cout << core_count << " cores detected.\n";

	// Store current time:
	timespec time1, time2;
	clock_gettime(CLOCK_MONOTONIC, &time1);

	// Populate global FDM I/O struct:
	cout << "Building struct...\n";
	input = new input_args();
	input->iterations = atoi(argv[6]);
	input->relaxation = atof(argv[7]);
	input->convergence = atof(argv[8]);
	input->lock = false;

	// Analyse bitmap, build initial array, add to global struct:
	cout << "Building array...\n";
	array_data * sys = locations(argv[1], atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]));
	input->sys = sys;

	// Run threader/solver:
	threader(core_count);

	// Calculate elapsed CPU time:
	clock_gettime(CLOCK_MONOTONIC, &time2);
	double runtime = long(diff(time1,time2).tv_sec) + 0.000000001*diff(time1,time2).tv_nsec;

	// Measure memory usage:
	int mem_used = mem_measure();
	cout << "Memory used: " << mem_used << " KB\n";

	// Output data to files:
	data_out(&sys, runtime);

	// Store image dimensions for plotting:
	int columns = sys->columns, rows = sys->rows;

	// Clean up memory, deleting both structs:
	delete sys -> prev_values;
	delete sys -> values;
	delete sys -> mask;
	delete sys -> xgrad;
	delete sys -> ygrad;
	delete sys;

	// delete input->sys;
	delete input->convcounts;
	delete input;

	// Plot data from files:
	cout << "Plotting data...\n";
	// Plot potential:
	plot(0,"pngcairo","png",columns,rows);
	// Plot E-field:
	plot(1,"pngcairo","png",columns,rows);
	// Plot equipotential lines:
	plot(2,"pngcairo","png",columns,rows);
	cout << "Done!\n";

	return 0;
}