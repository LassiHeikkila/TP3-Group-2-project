//////////////////////////////////////////////////////////
// Threadpool will be safely deleted when out of scope. //
//////////////////////////////////////////////////////////

#include "headers.h"

using namespace std;

void threader(int core_count)
{
	cout << "Creating pool of " << core_count << " worker threads...\n\n";
	// Simplify access to array struct:
	array_data* u = input->sys;
	input->convcounts = new int[core_count];

	// Initialise starting variables:
	int count = 0, convcount = 0, prev_convcount = 0;
	int pixels = u->rows * u->columns;

	// Create threadpool:
	boost::threadpool::pool thpool(core_count);

	while (convcount < pixels && count < input->iterations)
	{
		//cout << "Convcount: " << convcount << endl;
		// Reset convergence count, set red/black bool to red:
		convcount = 0;
		input->redblack = true;

		for (int i = 0; i < core_count; i++)
		{
			input->convcounts[i] = 0;
		}

		// Assign red elements to work queue with staggered
		// starting columns:
		for (int i = 0; i < core_count; i++)
		{
			// Add task to queue:
			thpool.schedule(boost::bind(&fdm,i,core_count));
		}

		// Wait for red threads:
		thpool.wait();

		// Set red/black bool to black:
		input->redblack = false;

		// Assign black elements to work queue:
		for (int j = 0; j < core_count; j++)
		{
			// Add task to queue:
			thpool.schedule(boost::bind(&fdm,j,core_count));
		}

		// Wait for black threads:
		thpool.wait();

		// Sum converged points:
		for (int i = 0; i < core_count; i++)
		{
			convcount += input->convcounts[i];
		}

		// Increment iteration count:
		count++;

		// Check for positive rate of change in convergence,
		// initiate locking mechanism:
		if (count % 100 == 0)
		{
			cout << "\rIteration " << count << "." << std::flush;
			//cout << "COUNT: " << data->count << ". CONV: " << data->convcount << endl;
			if (convcount > prev_convcount)
			{
				input->lock = true;
			}
		}

		// Set previous convergence to current:
		prev_convcount = convcount;

	}

	// Kill thread pool:

	// Store required iterations:
	u->req_its = count;

	// Build and populate gradient array:
	u->xgrad = new double*[u->columns];
	u->ygrad = new double*[u->columns];

	for (int i = 0; i < (u->columns - 1); i++)
	{
		u->xgrad[i] = new double[u->rows];
		u->ygrad[i] = new double[u->rows];

		for (int j = 0; j < (u->rows - 1); j++)
		{
			// Approximation of gradients:
			if (i != 0 && j != 0 )
			{
				u->xgrad[i][j] = -0.5*(u->values[i+1][j]-u->values[i-1][j]);
				u->ygrad[i][j] = -0.5*(u->values[i][j+1]-u->values[i][j-1]);
			}
		}

	}
	cout << endl;
}