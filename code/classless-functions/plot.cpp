//////////////////////////////////////////////////////
// Plots data files using user-defined filetype.    //
//                                                  //
// Requires /data and /plots folders to exist.      //
//                                                  //
// Options: 0 for potential plot, 1 for E-field.    //
//////////////////////////////////////////////////////

#include "headers.h"

void plot(int mode, string terminal, string file_ext)
{
    // Open GNUPlot as file to be written to:
    FILE *pipe_gnuplot = popen("gnuplot -persistent", "w");

    // Set terminal to user defined value:
    fputs(("set terminal "+terminal+"\n").c_str(), pipe_gnuplot);

    // Potential plot mode:
    if (mode == 0)
    {
        fputs(("set output 'plots/potential."+file_ext+"'\n").c_str(), pipe_gnuplot);
        fputs("set pm3d map\n", pipe_gnuplot);
        fputs("splot 'data/potential.dat'\n", pipe_gnuplot);
        fputs("set out\n", pipe_gnuplot);
    }
    // E-Field plot mode:
    else if (mode == 1)
    {
        fputs(("set output 'plots/field."+file_ext+"'\n").c_str(), pipe_gnuplot);
        fputs("plot 'data/field.dat' with vectors head filled lt 3\n", pipe_gnuplot);
        fputs("set out\n", pipe_gnuplot);
    }

    // Close GNUPlot:
    pclose(pipe_gnuplot);
}