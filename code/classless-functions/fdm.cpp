// Tests if element is equal to any boundary values. Not the best way though, since it'll treat all   //
// initial zeros as boundary if any boundary element is at ground.                                    //
//                                                                                                    //
// Keeping syntax just in case - will delete before final save...                                     //
// -----                                                                                              //
// std::any_of(std::begin(potentials), std::end(potentials), [&](double i){return i == u[i][j];}) == 1 //
// #include <algorithm>    // std::any_of()                                                           //

#include "headers.h"

void fdm(int startcol,int core_count)
{
	// Splitting input struct into variables to be manipulated:
	array_data* sysdat = input->sys;
	double**u = sysdat -> values;
	double**pu = sysdat -> prev_values;

	bool redblack = input->redblack;
	//bool lock = input->lock;

	double rel_par = input->relaxation;
	double desiredconv = input->convergence;

	double conv = 0;
	int start = 0;

	// Loop through assigned columns:
	for (int i = startcol; i < sysdat->columns; i += core_count)
	{
		// Check whether being assigned red or black elements, process
		// array accordingly:
		if (redblack)
		{
			start = (i == 0 || i % 2 == 0) ? 1 : 0;
		}
		else
		{
			start = (i != 0 && i % 2 != 0) ? 1 : 0;
		}

		// Loop through rows:
		for (int j = start; j < sysdat->rows; j += 2)
		{
			// Test if point is boundary value. If so, ignore: 
			if (sysdat->mask[i][j])
			{
				//cout << "Mask (" << i << ", " << j << "): " << sysdat->mask[i][j] << endl;
				//cout << "COUNT: " << input->count << ". CONVCOUNT: " << input->convcount << endl;				 
				input->convcounts[startcol] += 1;
				continue;
			}

			// Finite Difference Method to calculate potential as
			// average of surrounding values, checking if point is
			// on an edge. If so, and it isn't a boundary value,
			// calculate as average of surrounding points inside box
			// - two for corners, three for points on edge. If not on
			// edge, calculate as average of four surrounding points.

			// Left edge and corners:
			if (i == 0)
			{
				if (j == 0)
				{
					u[i][j] = (1-rel_par)*pu[i][j] + rel_par*0.5*(u[i+1][j]+u[i][j+1]);
				}
				else if (j == sysdat->rows - 1)
				{
					u[i][j] = (1-rel_par)*pu[i][j] + rel_par*0.5*(u[i+1][j]+u[i][j-1]);
				}
				else
				{
					u[i][j] = (1-rel_par)*pu[i][j] + rel_par*(1/3.0)*(u[i+1][j]+u[i][j+1]+u[i][j-1]);
				}
			}
			// Right edge and corners:
			else if (i == (sysdat->columns - 1))
			{
				if (j == 0)
				{
					u[i][j] = (1-rel_par)*pu[i][j] + rel_par*0.5*(u[i-1][j]+u[i][j+1]);
				}
				else if (j == sysdat->rows - 1)
				{
					u[i][j] = (1-rel_par)*pu[i][j] + rel_par*0.5*(u[i-1][j]+u[i][j-1]);
				}
				else
				{
					u[i][j] = (1-rel_par)*pu[i][j] + rel_par*(1/3.0)*(u[i-1][j]+u[i][j+1]+u[i][j-1]);
				}
			}
			// Top edge:
			else if (j == 0)
			{
				u[i][j] = (1-rel_par)*pu[i][j] + rel_par*(1/3.0)*(u[i-1][j]+u[i+1][j]+u[i][j+1]);
			}
			// Bottom edge:
			else if (j == (sysdat->rows - 1))
			{
				u[i][j] = (1-rel_par)*pu[i][j] + rel_par*(1/3.0)*(u[i-1][j]+u[i+1][j]+u[i][j-1]);
			}
			// Rest of array:
			else
			{
				u[i][j]=(1-rel_par)*pu[i][j] + rel_par*0.25*(u[i+1][j]+u[i][j+1]+u[i-1][j]+u[i][j-1]);
			}

			// Find absolute value of convergence:
			conv = fabs(pu[i][j] - u[i][j]);

			if (conv < desiredconv)
			{
				input->convcounts[startcol] += 1;

				if (input->lock && mintrue(sysdat,i,j,3))
				{
					cout << "LOCKED\n";
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