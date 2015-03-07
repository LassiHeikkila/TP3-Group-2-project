//////////////////////////////////////////////////////
// Plots data files using user-defined filetype.    //
//                                                  //
// Requires /data and /plots folders to exist.      //
//                                                  //
// Options: 0 for potential plot, 1 for E-field.    //
//          2 for equipotential lines only          //
//          3 for E-field and equipotentials        //
//////////////////////////////////////////////////////

#include "headers.h"

void plot(int mode, string terminal, string file_ext, int columns, int rows)
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
        fputs("set size 1,1\n",pipe_gnuplot);
        fputs("set origin 0,0\n",pipe_gnuplot);        
        fputs("set title 'Graph representing the potential across the grid'\n",pipe_gnuplot);
        fputs("set xlabel 'x-axis'\n",pipe_gnuplot);
        fputs("set ylabel 'y-axis'\n",pipe_gnuplot);
        fputs("set cblabel 'Potential'\n",pipe_gnuplot);
        fputs("splot 'data/potential.dat'\n", pipe_gnuplot);
        fputs("set out\n", pipe_gnuplot);
    }
    // E-Field plot mode:
    else if (mode == 1)
    {
        fputs(("set output 'plots/field."+file_ext+"'\n").c_str(), pipe_gnuplot);
        fputs("unset key\n",pipe_gnuplot);
        fputs("set tics\n",pipe_gnuplot);
        fputs("unset cbtics\n",pipe_gnuplot);
        fputs("set colorbox\n",pipe_gnuplot);
        fputs("set border 0\n",pipe_gnuplot);
        fprintf(pipe_gnuplot,"set xrange [0:%d]\n",columns); // have to use fprintf to insert an int into the middle of a string, it doesn't look as pretty but it works.
        fprintf(pipe_gnuplot,"set yrange [0:%d]\n",rows);
    //  fputs(("set xrange [0:%d]\n",columns),pipe_gnuplot);
    //  fputs(("set yrange [0:%d]\n",rows),pipe_gnuplot);
        fputs("set title 'Vector plot representing the electric field across the grid'\n",pipe_gnuplot);
        fputs("set xlabel 'x-axis'\n",pipe_gnuplot);
        fputs("set ylabel 'y-axis'\n",pipe_gnuplot);
        fputs("set cblabel 'Magnitude'\n",pipe_gnuplot);
        fputs("set palette defined ( 0 \"blue\", 1 \"red\")\n",pipe_gnuplot);
        fputs("scaling = 7\n",pipe_gnuplot);
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
        fputs("set cntrparam levels incremental -15,1,15\n",pipe_gnuplot);
        fputs("splot 'data/potential.dat' w l ls 1\n",pipe_gnuplot);
    }

    else if (mode == 3)
    {
        // set output file to field.png
        fputs(("set output 'plots/field_and_equipotentials."+file_ext+"'\n").c_str(), pipe_gnuplot);

        // multiplot required to plot multiple graphs
        fputs("set multiplot\n",pipe_gnuplot);

        fputs("unset key\n",pipe_gnuplot);
        fputs("set tics\n",pipe_gnuplot);
        fputs("unset cbtics\n",pipe_gnuplot);
        fputs("set colorbox\n",pipe_gnuplot);
        fputs("set border 0\n",pipe_gnuplot);
        fputs("set size 1,1\n",pipe_gnuplot);
        fprintf(pipe_gnuplot,"set xrange [0:%d]\n",columns); // have to use fprintf to insert an int into the middle of a string, it doesn't look as pretty but it works.
        fprintf(pipe_gnuplot,"set yrange [0:%d]\n",rows);
        //  fputs(("set xrange [0:%d]\n",columns),pipe_gnuplot);
        //  fputs(("set yrange [0:%d]\n",rows),pipe_gnuplot);

        // Title and labels
        fputs("set title 'Vector plot representing the electric field across the grid with equipotential lines overlaid'\n",pipe_gnuplot);
        fputs("set xlabel 'x-axis'\n",pipe_gnuplot);
        fputs("set ylabel 'y-axis'\n",pipe_gnuplot);
        fputs("set cblabel 'Magnitude'\n",pipe_gnuplot);

        // define palette
        fputs("set palette defined ( 0 \"blue\", 1 \"red\")\n",pipe_gnuplot);
        
        // length of arrows
        fputs("scaling = 7\n",pipe_gnuplot);
        
        // sizing the arrows
        fputs("mag(x,y) = sqrt( x*x + y*y )\n",pipe_gnuplot);
        fputs("normx(x,y) = scaling * ( x / mag(x,y) )\n",pipe_gnuplot);
        fputs("normy(x,y) = scaling * ( y / mag(x,y) )\n",pipe_gnuplot);
        
        // plot the vector field
        fputs("plot 'data/field.dat' using ($1):($2):(normx($3,$4)):(normy($3,$4)):(mag($3,$4)) with vectors head filled size screen 0.005,30,60 lc palette\n", pipe_gnuplot);
        
        // clear contour plot of extra junk
        fputs("unset tics\n",pipe_gnuplot);
        fputs("unset title\n",pipe_gnuplot);
        fputs("unset xlabel\n",pipe_gnuplot);
        fputs("unset ylabel\n",pipe_gnuplot);

        // plot contours and output as table
        fputs("set view map\n",pipe_gnuplot);
        fputs("unset surface\n",pipe_gnuplot);
        fputs("set contour base\n",pipe_gnuplot);
        fputs("set cntrparam levels incremental -11,1,10\n",pipe_gnuplot);
        fputs("set table 'data/contour'\n",pipe_gnuplot);
        fputs("splot 'data/potential.dat' w l ls 1\n",pipe_gnuplot);
        fputs("unset table\n",pipe_gnuplot);

        // manually adjust positioning of contour plot
        fputs("set size 0.84,0.88\n",pipe_gnuplot);
        fputs("set origin 0.04,0.06\n",pipe_gnuplot);

        // set height at which labels appear and the width of the gap in the contour lines at the labels
        fputs("height_label = 28\n",pipe_gnuplot);
        fputs("height_gap = 10\n",pipe_gnuplot);

        // calculate positioning of labels and gaps
        fputs("labels(x,y) = (y == height_label) ? stringcolumn(3) : \"\"\n",pipe_gnuplot);
        fputs("f(x,y) = ( abs( y - height_label ) < (height_gap / 2) ) ? 1/0 : y\n",pipe_gnuplot);

        // plot contours and labels
        fputs("plot 'data/contour' u 1:(f($1,$2)) with lines\n",pipe_gnuplot);
        fputs("plot 'data/contour' u 1:2:(labels($1,$2)) with labels\n",pipe_gnuplot);

        // unset multiplot
        fputs("unset multiplot\n",pipe_gnuplot);
    }
    else
    {
        std::cout << "Invalid mode on plot()." << std::endl;
    }

    // Close GNUPlot:
    pclose(pipe_gnuplot);
}
