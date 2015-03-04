#include "headers.h"

pthread_barrier_t waiter;

bool startbool = true;

array_data * threader(array_data* u, double relaxation, double convergence, int iterations, int core_count)
{
	// Build input data struct, to be passed to
	// solver threads:
	cout << "Building struct...\n";
	input_args* data = new input_args();
	data->sys = u;
	data->relaxation = relaxation;
	cout << "Relaxation at threader: " << data->relaxation << " = " << relaxation << endl;
	data->convergence = convergence;
	cout << "Convergence at threader: " << data->convergence << " = " << convergence << endl;
	data->iterations = iterations;
	data->t_step = core_count;
	data->lock = false;
	u->req_its = 0;
	cout << "Done!\n";
	data->prev_convcount = 0;

	int pixels = u->rows * u->columns;

	cout << pixels << " pixels.\n";
	cout << data->iterations << " iterations.\n";

	pthread_t threads[core_count];
	//pthread_t blackthreads[core_count];
	if (pthread_barrier_init(&waiter,NULL,core_count))
	{
		cout << "Barrier initialisation failed!\n";
	}

	data->convcounts = new int[core_count];

	int rc;

	while ( data->convcount < pixels && data->count < 2*iterations )
	{
		data->convcount = 0;

		for (int i = 0; i < core_count; i++)
		{
			data->convcounts[i] = 0;
		}

		// data->zeroconvcount = 0;
		// data->oneconvcount = 0;				

		data->redblack = true;

		for (int i = 0; i < core_count; i++)
		{
			//cout << "Starting position: " << data->t_start << endl;
			//cout << "Running thread " << i << " (RED).\n";
			redbackup:
			if (startbool)
			{
				startbool = false;
				data->t_start = i;
				rc = pthread_create(&threads[i], NULL, fdm, (void *)data);
			}
			else
			{
				goto redbackup;
			}
			if (rc)
			{
				cout << "Error creating thread, please restart.\n";
				exit(-1);
			}
		}

		for(int i = 0; i < core_count; i++)
		{
			pthread_join(threads[i], NULL);
		}

		data->redblack = false;

		for (int j = 0; j < core_count; j++)
		{
			//cout << "Starting position: " << data->t_start << endl;
			//cout << "Running thread " << j << " (BLACK).\n";
			blackbackup:
			if (startbool)
			{
				startbool = false;
				data->t_start = j;
				rc = pthread_create(&threads[j], NULL, fdm, (void *)data);
			}
			else
			{
				goto blackbackup;
			}
			if (rc)
			{
				cout << "Error creating thread, please restart.\n";
				exit(-1);
			}
		}
	
		//pthread_barrier_wait(&waiter);
		for(int i = 0; i < core_count; i++)
		{
			pthread_join(threads[i], NULL);
		}

		for (int i = 0; i < core_count; i++)
		{
			data->convcount += data->convcounts[i];
		}
		// data->convcount += data->zeroconvcount + data->oneconvcount;
		// data->zeroconvcount = 0;
		// data->oneconvcount = 0;	
		data->count += 1;

		if (data->count % 200 == 0)
		{
			cout << "\rIteration " << data->count / 2 << "." << std::flush;
			// cout << "COUNT: " << data->count << ". CONV: " << data->convcount << endl;
			if (data->convcount > data->prev_convcount)
			{
				//cout << "LOCK ON\n";
				data->lock = true;
			}
		}

		data->prev_convcount = data->convcount;
	}

	u->req_its = data->count / 2;
	// Pass iterations required to struct:
	// sysdat->req_its = in_data->count / 2;
	// Create 2D arrays for grad:
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
	
	return u;
}