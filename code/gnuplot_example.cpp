// Simple example of how to run gnuplot from within a C++ program (actually is just C code)
// Each gnuplot command must be fprintf'fed to whatever file is popen'ed for it
// (in this case "gnuplotpipe" is the file, then commands are fed into that file)
// Can be thrown into the end of the program.
// This version plots two graphs, each corresponding to one of the following data files: field.dat and potential.dat

// By Lassi Heikkilä

// HAVE TO INCLUDE stdio.h

{
// Remember to add \n after each line
  FILE* gnuplotpipe = popen ("gnuplot -persistent", "w"); // popen a file called gnuplotpipe in write mode to run gnuplot
    
  fprintf(gnuplotpipe, "set terminal postscript landscape enhanced color\n"); // set terminal to whatever
  fprintf(gnuplotpipe, "set output 'field.eps'\n"); // set output to field.eps
  fprintf(gnuplotpipe, "set xrange[-1:10]\nset yrange[-1:10]\n"); // set xrange and yrange
  fprintf(gnuplotpipe, "plot 'field.dat' with vectors head filled lt 3\n"); // plot field.dat with vectors
  fprintf(gnuplotpipe, "set output 'potential.eps'\n"); // set output to potential.eps
  fprintf(gnuplotpipe, "set pm3d map\n"); // set output to heat-map type thing
  fprintf(gnuplotpipe, "splot 'potential.dat'\n"); // plot potential.dat
  
  pclose(gnuplotpipe); // pclose the file
  
}
