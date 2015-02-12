#include "headers.h"

void data_out(array_data**u, double time_elapsed, int iterations)
{
    cout << "Starting data_out.\n";
    cout << "Time: " << time_elapsed << endl;
    cout << "iterations: " << iterations << endl;
    cout << "Value at (0,0): " << (*u)->values[0][0] << endl;

    cout << "X Grad at (0,0): " << (*u)->xgrad[0][0] << endl;
    cout << "Y Grad at (0,0): " << (*u)->ygrad[0][0] << endl;

    
    // Append N against elapsed time to file:
    ofstream runtime("data/runtime.dat", ios::out | ios::app);
    runtime << setw(10) << iterations << "\t" << setw(10) << time_elapsed << endl;
    runtime.close();

    ofstream potential("data/potential.dat");
    ofstream field("data/field.dat");

    // Loop through final arrays, writing data to file:
    for (int i = 0; i < (*u)->columns; i++)
    {
        for (int j = 0; j < (*u)->rows; j++)
        {
            // Write potential to file:
            potential << setw(10) << i << "\t" << setw(10) << j << "\t" << setw(10) << (*u) -> values[i][j] << endl;

            if (i == (*u)->columns - 1 || j == (*u)->rows - 1)
            {
                continue;
            }
            //print out x, y, -du/dx, -du/dy in tabular form (components of E field)
            field << setw(10) << i << "\t" << setw(10) << j << "\t" << setw(10) << (*u) -> xgrad[i][j] << "\t" << setw(10) << (*u) -> ygrad[i][j] << endl;
        }

        potential << endl;

    }

    potential.close();
    field.close();

    cout << "FILES FINISHED.\n";
}
