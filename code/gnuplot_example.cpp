// Simple example of how to run gnuplot from within a C++ program (actually is just C code)
// Each gnuplot command must be fprintf'fed to whatever file is popen'ed for it
// (in this case "gnuplotpipe" is the file, then commands are fed into that file)
//
// By Lassi Heikkilä

#include <stdio.h>


int main(int argc, char *argv[])
{
  FILE* gnuplotpipe = popen ("gnuplot -persistent", "w"); // popen a file called gnuplotpipe in write mode to run gnuplot
  fprintf(gnuplotpipe, "set terminal postscript\n");      // example gnuplot command
  fprintf(gnuplotpipe, "set output 'sin.eps'\n");         // example gnuplot command
  fprintf(gnuplotpipe, "plot sin(x)");                    // example gnuplot command
  pclose(gnuplotpipe);                                    // pclose the file
  
  return 0;                                               // exit program. a file called 'sin.eps' will have appeared in working directory.
}
