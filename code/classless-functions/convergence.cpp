#include "headers.h"

void converge(int startcol, int core_count)
{
	double conv = 0, desiredconv = input->convergence;

	array_data* sysdat = input->sys;
	double**u = sysdat->values;
	double**pu = sysdat->prev_values;

	for (int i = startcol; i < sysdat->columns; i += core_count)
	{
		for (int j = 0; j < sysdat->rows; j++)
		{
			if (sysdat->mask[i][j])
			{
				continue;
			}

			// Find absolute value of convergence:
			conv = fabs(pu[i][j] - u[i][j]);

			if (conv < desiredconv)
			{
				input->convcounts[startcol] += 1;

				if (input->lock && mintrue(sysdat,i,j,2))
				{
					sysdat->mask[i][j] = true;
				}
			}

			pu[i][j] = u[i][j];
		}
	}
}

// Function to determine if at least some minimum number
// of bools are true.
bool mintrue(array_data*u,int i, int j, int min)
{
	int count = 0;

	if (i == 0 || i == u->columns - 1 || j == 0 || j == u->rows - 1)
	{
		return false;
	}

	for (int x = -1; x < 2; x += 2)
	{
		count += u->mask[i+x][j] ? 1 : 0;
		count += u->mask[i][j+x] ? 1 : 0;
	}

	return (count >= min);
}