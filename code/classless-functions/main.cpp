/********************************************

Command line arguments:
0 - program name
1 - image name
2 - potential for black pixels
3 - potential for red pixels
4 - potential for green pixels
5 - potential for blue pixels
6 - maximum number of iterations
7 - relaxation parameter (recommended values between 1 and 2)
8 - desired convergence

**********************************************/


#include "headers.h"

int main(int argc, char *argv[])
{
    // Declare potential array, initialise with command line arguments:
    double potentials[4] = {atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5])};

    // Initialise other starting variables:
    int iterations = atoi(argv[6]);
    double relaxation = atof(argv[7]);
    double convergence = atof(argv[8]);
    
    // Store current CPU time:
    clock_t tm;
    tm = clock();

    // Run solver:
    array_data * data = fdm(argv[1], potentials, relaxation, iterations, convergence);

    // Calculate elapsed CPU time:
    tm = clock() - tm;
    // Convert to seconds:
    double runtime = double(tm) / CLOCKS_PER_SEC;

    // Measure memory used by process:
    int mem_used = mem_measure();

    cout << "Memory used: " << mem_used << " KB\n";

    // Output data to files:
    data_out(&data, runtime);

    // Clean up memory:
    delete data -> prev_values;
    delete data -> values;
    delete data -> mask;
    delete data -> xgrad;
    delete data -> ygrad;
    delete data;

    cout << "Plotting data...\n";
    // Plot potential:
    plot(0,"pngcairo","png");
    // Plot E-field:
    plot(1,"pngcairo","png");
    // Plot equipotential lines:
    plot(2,"pngcairo","png");
    cout << "Done!\n";

    return 0;
}
