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
    fputs(("set terminal "+terminal+" size 1920,1080\n").c_str(), pipe_gnuplot);

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
        fputs("unset key\n",pipe_gnuplot);
        fputs("unset tics\n",pipe_gnuplot);
        fputs("unset colorbox\n",pipe_gnuplot);
        fputs("set border 0\n",pipe_gnuplot);
        fputs("set xrange [0:400]\n",pipe_gnuplot);
        fputs("set yrange [0:300]\n",pipe_gnuplot);
        fputs("set palette defined ( 0 \"blue\", 1 \"red\")\n",pipe_gnuplot);
        fputs("scaling = 5\n",pipe_gnuplot);
        fputs("mag(x,y) = sqrt( x*x + y*y )\n",pipe_gnuplot);
        fputs("normx(x,y) = scaling * ( x / mag(x,y) )\n",pipe_gnuplot);
        fputs("normy(x,y) = scaling * ( y / mag(x,y) )\n",pipe_gnuplot);
        fputs("plot 'data/field.dat' using ($1):($2):(normx($3,$4)):(normy($3,$4)):(mag($3,$4)) with vectors head filled size screen 0.005,30,60 lc palette\n", pipe_gnuplot);
    }
    else if (mode == 2)
    {
        fputs(("set output 'plots/equipotential."+file_ext+"'\n").c_str(),pipe_gnuplot);
        fputs("set key outside\n",pipe_gnuplot);
        fputs("unset tics\n",pipe_gnuplot);
        fputs("set view map\n",pipe_gnuplot);
        fputs("unset surface\n",pipe_gnuplot);
        fputs("set contour base\n",pipe_gnuplot);
        fputs("set cntrparam levels incremental -11,1,10\n",pipe_gnuplot);
        fputs("splot 'data/potential.dat' w l ls 1\n",pipe_gnuplot);
    }

    // Close GNUPlot:
    pclose(pipe_gnuplot);
}
