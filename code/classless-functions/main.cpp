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
//    double potentials[4] = {atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5])};

    // Initialise other starting variables:
    int iterations = atoi(argv[6]);
    double relaxation = atof(argv[7]);
    double convergence = atof(argv[8]);

    int core_count = std::thread::hardware_concurrency();

    cout << core_count << " cores.\n";

    // Store current CPU time:
    clock_t tm;
    tm = clock();

    // Build array from bitmap:
    cout << "Building array...\n";
    array_data * data = locations(argv[1], atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]));
    cout << "Done!\n";
    // Run solver:
    cout << "Threading...\n";
    data = threader(data, relaxation, convergence, iterations, core_count);
    cout << "Done!\n";
    // Calculate elapsed CPU time:
    tm = clock() - tm;
    // Convert to seconds:
    double runtime = double(tm) / CLOCKS_PER_SEC;

    // Measure memory used by process:
    int mem_used = mem_measure();

    cout << "Memory used: " << mem_used << " KB\n";

    // Output data to files:
    data_out(&data, runtime);

     // Store width and height of image for plotting purposes
    int columns = data->columns;
    int rows = data->rows;

    // Clean up memory:
    delete data -> prev_values;
    delete data -> values;
    delete data -> mask;
    delete data -> xgrad;
    delete data -> ygrad;
    delete data;

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
