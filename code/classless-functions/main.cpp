#include "headers.h"

int main(int argc, char *argv[])
{
	// Declare potential array:
	float potentials[4];

	// Assign command line args to corresponding boundary potential
	// array:
    potentials[0] = atof(argv[2]), potentials[1] = atof(argv[3]);
    potentials[2] = atof(argv[4]), potentials[3] = atof(argv[5]);

    // Final command line argument is iteration count:
    int iterations=atoi(argv[6]);
    float relaxation = atof(argv[7]);
    float convergence = atof(argv[8]);

    // Store current CPU time:
	clock_t tm;
	tm = clock();

    // Run solver:
    array_data * data = fdm(argv[1], potentials, relaxation, iterations, convergence);

	// Calculate elapsed CPU time:
	tm = clock() - tm;
	// Convert to seconds:
	double runtime = double(tm) / CLOCKS_PER_SEC;

    // Output data to files:
    data_out(&data, runtime, iterations);

    cout << "Data DONE.\n";

    // Clean up memory:
    delete data -> prev_values;
    delete data -> values;
    delete data -> mask;
    delete data -> xgrad;
    delete data -> ygrad;
    delete data;

    // Plot potential:
    plot(0,"pngcairo","png");
    // Plot E-field:
    plot(1,"pngcairo","png");

	return 0;
}
