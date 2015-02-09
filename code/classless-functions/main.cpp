using namespace std;

int main(int argc, char const *argv[])
{
	// Declare potential array:
	float*potentials[4];

	// Assign command line args to corresponding boundary potential
	// array:
  potentials[0] = atof(argv[2]), potentials[1] = atof(argv[3]);
  potentials[2] = atof(argv[4]), potentials[3] = atof(argv[5]);

  // Final command line argument is iteration count:
  int iterations=atoi(argv[6]);

  // Store current CPU time:
	clock_t tm;
	tm = clock();

  // Run solver:
  array_data ** data = fdm(argv[1], potentials, iterations);

	// Calculate elapsed CPU time:
	tm = clock() - tm;
	// Convert to seconds:
	double runtime = double(tm) / CLOCKS_PER_SEC;

  // Output data to files:
  data_out(data, runtime, iterations);

  // Clean up memory:
  delete data -> values;
  delete data -> mask;
  delete data -> xgrad;
  delete data -> ygrad;

  // Plot potential:
  plot(0,"pngcairo","png");
  // Plot E-field:
  plot(1,"pngcairo","png");

	return 0;
}
