// Tests if element is equal to any boundary values. Not the best way though, since it'll treat all   //
// initial zeros as boundary if any boundary element is at ground.                                    //
//                                                                                                    //
// Keeping syntax just in case - will delete before final save...                                     //
// -----                                                                                              //
// std::any_of(std::begin(potentials), std::end(potentials), [&](float i){return i == u[i][j];}) == 1 //
// #include <algorithm>    // std::any_of()                                                           //

array_data * fdm(char* image, float* potentials, int iterations)
{
    // Use locations() to build initial array with boundaries:
    array_data * sysdat;
    sysdat = locations(image, p_black, p_red, p_green, p_blue);

    float**u = sysdat -> values;
    bool**bound_mask = sysdat -> mask;

    float**x_grad = sysdat -> xgrad;
    float**y_grad = sysdat -> ygrad;

    // Loop through desired iteration count:
    for (int count = 0; count < iterations; count++)
    {
        // Loop through x coordinates 
        for (int i = 0; i < sysdat.columns; i++)
        {
            // Loop through y:
            for (int j = 0; j < sysdat.rows; j++)
            {
                // Test if point is boundary value. If so, ignore:
                if (bound_mask[i][j])
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
                if (i = 0)
                {
                    if (j = 0)
                    {
                        u[i][j] = 0.5*(u[i+1][j]+u[i][j+1]);
                    }
                    else if (j = rows - 1)
                    {
                        u[i][j] = 0.5*(u[i+1][j]+u[i][j-1]);
                    }
                    else
                    {
                        u[i][j]=(1/3.0)*(u[i+1][j]+u[i][j+1]+u[i][j-1]);
                    }
                }
                // Right edge and corners:
                else if (i = (columns - 1))
                {
                    if (j = 0)
                    {
                        u[i][j] = 0.5*(u[i-1][j]+u[i][j+1]);
                    }
                    else if (j = rows - 1)
                    {
                        u[i][j] = 0.5*(u[i-1][j]+u[i][j-1]);
                    }
                    else
                    {
                        u[i][j]=(1/3.0)*(u[i-1][j]+u[i][j+1]+u[i][j-1]);
                    }
                }
                // Top edge:
                else if (j = 0)
                {
                    u[i][j]=(1/3.0)*(u[i-1][j]+u[i+1][j]+u[i][j+1]);
                }
                // Bottom edge:
                else if (j = (rows - 1))
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
    x_grad = new float*[sysdat.columns][sysdat.rows];
    y_grad = new float*[sysdat.columns][sysdat.rows];

    for (int j = 0; j <= (sysdat.rows - 1); j++)
    {
        for (int i=0; i<=(sysdat.columns - 1); i++)
        {
            // Approximation of gradients:
            x_grad[i][j] = (u[i+1][j]-u[i][j]);
            y_grad[i][j] = (u[i][j+1]-u[i][j]);
        }

    }

    return sysdat;
}
