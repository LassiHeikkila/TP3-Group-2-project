array_data * fdm(char* image, double* potentials, double rel_par, int iterations, double desiredconv)
{
    // Use locations() to build initial array with boundaries:
    array_data * sysdat;
    sysdat = locations(image, potentials[0], potentials[1], potentials[2], potentials[3]);

    double conv = 0;
    int prev_convcount = 0, convcount = 0, count = 0;
    bool lock = false;
    int pixels = sysdat->rows * sysdat->columns;

    double**u = sysdat -> values;
    double**pu = sysdat -> prev_values;

	if (red_black == 0) // red.
	{
		int start = (i != 0 && i % 2 != 0) ? 1 : 0;
	}
	else if (red_black == 1) // black.
	{
		int start = (i == 0 || i % 2 == 0) ? 1 : 0;
	}

	while (convcount < pixels && count < iterations)
	{
		convcount = 0;

		for (int i = 0; i < sysdat->columns; i++)
		{
			if (count % 2 == 0)
			{
				int start = (i != 0 && i % 2 != 0) ? 1 : 0;
			}
			else
			{
				int start = (i == 0 || i % 2 == 0) ? 1 : 0;
			}
			
			for (int j = start; j < sysdat->rows; i+=2)
			{
				if (sysdat->mask[i][j])
				{
					convcount++;
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
                // Rest of area:
                else
                {
                    u[i][j]=(1-rel_par)*pu[i][j] + rel_par*0.25*(u[i+1][j]+u[i][j+1]+u[i-1][j]+u[i][j-1]);
                }


				// Find absolute value of convergence:
				conv = abs(pu[i][j] - u[i][j]);

				if (conv < desiredconv)
				{
					convcount++;

					if (lock && mintrue(sysdat,i,j,2))
					{
					    sysdat->mask[i][j] = true;
					}
				}

				pu[i][j] = u[i][j];
	            
	            }
	        }

			count++;

			if (count % 100 == 0)
			{
				cout << "\rIteration " << count << "." << std::flush;

				if (convcount > prev_convcount)
				{
					lock = true;
				}
			}

			prev_convcount = convcount;

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