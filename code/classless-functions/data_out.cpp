void data_out(array_data**u, double time_elapsed, int iterations)
{
  // Loop through final arrays, writing data to file:
    for (int j=0; j<=(u.rows -1); j++)
    {
        for (int i=0; i<=(u.columns - 1); i++)
        {
            // Append N against elapsed time to file:
            ofstream file("data/runtime.dat", ios::out | ios::app);
            file << setw(10) << iterations << "\t" << setw(10) << time_elapsed << endl;

            // Write potential to file:
            ofstream file("data/potential.dat", ios::out | ios::app);
            file << setw(10) << i << "\t" << setw(10) << j << "\t" << setw(10) << u -> values[i][j] << endl;

            //print out x, y, -du/dx, -du/dy in tabular form (components of E field)
            ofstream file("data/field.dat", ios::out | ios::app);
            file << setw(10) << i << "\t" << setw(10) << j << "\t" << setw(10) << -1*(u -> xgrad[i][j]) << "\t" << setw(10) << -1*(u -> ygrad[i][j]) << endl;
        }

    }
}
