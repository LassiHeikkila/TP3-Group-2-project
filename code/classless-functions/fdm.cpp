// Tests if element is equal to any boundary values. Not the best way though, since it'll treat all   //
// initial zeros as boundary if any boundary element is at ground.                                    //
//                                                                                                    //
// Keeping syntax just in case - will delete before final save...                                     //
// -----                                                                                              //
// std::any_of(std::begin(potentials), std::end(potentials), [&](double i){return i == u[i][j];}) == 1 //
// #include <algorithm>    // std::any_of()                                                           //

#include "headers.h"

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

    // Loop until either desired convergence or maximum
    // iterations are achieved:
    while ( convcount < pixels && count < iterations )
//    for (int count = 0; count < iterations; count++)
    {
        convcount = 0;

        // Loop through x coordinates 
        for (int i = 0; i < sysdat->columns; i++)
        {
            // Loop through y:
            for (int j = 0; j < sysdat->rows; j++)
            {
                // Test if point is boundary value. If so, ignore:
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
                        u[i][j]=(1-rel_par)*pu[i][j] + rel_par*(1/3.0)*(u[i+1][j]+u[i][j+1]+u[i][j-1]);
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
//                    sysdat->mask[i][j] = 1;
                    convcount++;

                    if (lock && (sysdat->mask[i-1][j] || sysdat->mask[i+1][j] || sysdat->mask[i][j-1] || sysdat->mask[i][j+1]))
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

    cout << endl;

    // Pass iterations required to struct:
    sysdat->req_its = count;
    // Create 2D arrays for grad:
    sysdat->xgrad = new double*[sysdat->columns];
    sysdat->ygrad = new double*[sysdat->columns];

    for (int i = 0; i < (sysdat->columns - 1); i++)
    {
        sysdat->xgrad[i] = new double[sysdat->rows];
        sysdat->ygrad[i] = new double[sysdat->rows];

        for (int j = 0; j < (sysdat->rows - 1); j++)
        {
            // Approximation of gradients:
            sysdat->xgrad[i][j] = -1*(u[i+1][j]-u[i][j]);
            sysdat->ygrad[i][j] = -1*(u[i][j+1]-u[i][j]);
        }

    }
    return sysdat;
}
