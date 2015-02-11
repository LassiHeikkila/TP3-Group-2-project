// Tests if element is equal to any boundary values. Not the best way though, since it'll treat all   //
// initial zeros as boundary if any boundary element is at ground.                                    //
//                                                                                                    //
// Keeping syntax just in case - will delete before final save...                                     //
// -----                                                                                              //
// std::any_of(std::begin(potentials), std::end(potentials), [&](float i){return i == u[i][j];}) == 1 //
// #include <algorithm>    // std::any_of()                                                           //

#include "headers.h"

array_data * fdm(char* image, float* potentials, int iterations)
{
    // Use locations() to build initial array with boundaries:
    array_data * sysdat;
    sysdat = locations(image, potentials[0], potentials[1], potentials[2], potentials[3]);

    float**u = sysdat -> values;
//    bool**bound_mask = sysdat -> mask;

//    float**x_grad = sysdat -> xgrad;
//    float**y_grad = sysdat -> ygrad;

    // Loop through desired iteration count:
    for (int count = 0; count < iterations; count++)
    {
        if (count % 500 == 0)
        {
            cout << "Iteration " << count << ".\n";
        }
        // Loop through x coordinates 
        for (int i = 0; i < sysdat->columns; i++)
        {
            // Loop through y:
            for (int j = 0; j < sysdat->rows; j++)
            {
                // Test if point is boundary value. If so, ignore:
                if (sysdat->mask[i][j])
                {
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
                        u[i][j] = 0.5*(u[i+1][j]+u[i][j+1]);
                    }
                    else if (j == sysdat->rows - 1)
                    {
                        u[i][j] = 0.5*(u[i+1][j]+u[i][j-1]);
                    }
                    else
                    {
                        u[i][j]=(1/3.0)*(u[i+1][j]+u[i][j+1]+u[i][j-1]);
                    }
                }
                // Right edge and corners:
                else if (i == (sysdat->columns - 1))
                {
                    if (j == 0)
                    {
                        u[i][j] = 0.5*(u[i-1][j]+u[i][j+1]);
                    }
                    else if (j == sysdat->rows - 1)
                    {
                        u[i][j] = 0.5*(u[i-1][j]+u[i][j-1]);
                    }
                    else
                    {
                        u[i][j]=(1/3.0)*(u[i-1][j]+u[i][j+1]+u[i][j-1]);
                    }
                }
                // Top edge:
                else if (j == 0)
                {
                    u[i][j]=(1/3.0)*(u[i-1][j]+u[i+1][j]+u[i][j+1]);
                }
                // Bottom edge:
                else if (j == (sysdat->rows - 1))
                {
                    u[i][j]=(1/3.0)*(u[i-1][j]+u[i+1][j]+u[i][j-1]);
                }
                // Rest of area:
                else
                {
                    u[i][j]=0.25*(u[i+1][j]+u[i][j+1]+u[i-1][j]+u[i][j-1]);
                }
            }
        }
    }

    // Create 2D arrays for grad:
    sysdat->xgrad = new float*[sysdat->columns];//[sysdat->rows];
    sysdat->ygrad = new float*[sysdat->columns];//[sysdat->rows];

    for (int i = 0; i < (sysdat->columns - 1); i++)
    {
        sysdat->xgrad[i] = new float[sysdat->rows];
        sysdat->ygrad[i] = new float[sysdat->rows];

        for (int j = 0; j < (sysdat->rows - 1); j++)
        {
//            cout << "Finding grads at (" << i << "," << j << ").\n";
            // Approximation of gradients:
            sysdat->xgrad[i][j] = -1*(u[i+1][j]-u[i][j]);
            sysdat->ygrad[i][j] = -1*(u[i][j+1]-u[i][j]);
        }

    }
    cout << "GRAD FINISHED.\n";
    return sysdat;
}
