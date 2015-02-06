/*

Couple of things to note: setting array size to 5000x5000 (e.g. x=10,dx=0.002,h=10,dy=0.002) produces an output.dat array which is 237.5 megabytes.
Even then, it only took a virtual machine running ubuntu with 1GB of ram and utilising maybe 1 core of an i7 processor 2 minutes and 18 seconds to complete 100 iterations. 
Clearly, this program is able to do massive calculations with quite modest computing power, although it was taking up ~ 89 % of the CPU according to htop.

 */

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include "bitmap_image.hpp"
#include "class_Laplace.h"
// #include "boundary_reader.h"


int iterations;
float d, dx, h, dy, r;

float locations(char* file); // used to locate the positions and types of boundary conditions

// Main function
int main(int argc, char *argv[])
{

  // float d  = atof(argv[1]); // plate separation
  //float dx = atof(argv[2]); // step size in x
  //float h  = atof(argv[3]); // plate height
  //float dy = atof(argv[4]); // step size in y
  //float r  = atof(argv[5]); // radius of cylinder
  int iterations = atoi(argv[6]); // number of iterations
  float potential_black = atoi(argv[7]);  // potential at black pixels
  float potential_red   = atoi(argv[8]);  // potential at red pixels
  float potential_green = atoi(argv[9]);  // potential at green pixels
  float potential_blue  = atoi(argv[10]); // potential at blue pixels

  // float locate = locations(argv[11]); // filename

  // validate input by checking all given arguments are sensible
  /* if ( d < 0 || dx < 0 || h < 0 || dy < 0 || dx > d || dy > h || r >= d/2 || r >= h/2 || iterations < 1)
    {
      std::cout << "Invalid input. Exiting now!" << std::endl;
      exit(EXIT_FAILURE);
    }
  */

  Laplace pixels;
  bitmap_image image(argv[11]);
  if (!image)
    {
      std::cout << "Unable to open file." << std::endl;
      exit(EXIT_FAILURE);
    }
  pixels.SetArraySize(image.width(),image.height());

  float d = image.width();
  float h = image.height();
  float dx = 1;
  float dy = 1;
  float r = 1; 

  unsigned char red,green,blue;

  Laplace u;
  u.SetArraySize(d,h);
  
  for ( unsigned int p = 0 ; p < d ; p++ )
    {
      for ( unsigned int q = 0 ; q < h ; q++ )
	{
	  // analyse RGB values of pixel at (p,q):
	  image.get_pixel(p,q,red,green,blue);
	  // depending on colour of pixel, set potential to user defined values
	  
	  // Black pixels:
	  if ( red < 10 && blue < 10 && green < 10 )
	    {
	      pixels.array[p][q] = 1;
	    }
	  // Red pixels:
	  else if ( red > blue && red > green )
	    {
	      pixels.array[p][q] = 2;
	    }
	  // Green pixels:
	  else if ( green > blue && green > red )
	    {
	      pixels.array[p][q] = 3;
	    }
	  // Blue pixels:
	  else if ( blue > red && blue > green )
	    {
	      pixels.array[p][q] = 4;
	    }
	  else
	    {
	      pixels.array[p][q] = 0;
	    }
	}
    }
  // pixels.ViewArray();
  // iterate over the grid until count reaches 'iterations'
  for ( int count = 0 ; count < iterations ; count++ )
    {
      for ( unsigned int i = 1 ; i < d-1 ; i++ )
	{
	  for ( unsigned int j = 1 ; j-1 < h ; j++ )
	    {
	      if ( pixels.array[i][j] == 1 )
		{
		  u.array[i][j] = potential_black;
		}
	      else if ( pixels.array[i][j] == 2 )
		{
		  u.array[i][j] = potential_red;
		}
	      else if ( pixels.array[i][j] == 3 )
		{
		  u.array[i][j] = potential_green;
		}
	      else if ( pixels.array[i][j] == 4 )
		{ 
		  u.array[i][j] = potential_blue;
		}
	      else 
		{
		  u.array[i][j] = 0.25 * (u.array[i-1][j] + u.array[i+1][j] + u.array[i][j-1] + u.array[i][j+1]);
		}
	    }
	}
    }
  
  // u.ViewArray(); // Uncomment this to see a bunch of meaningless garbage on your screen (not actually meaningless, they are the values of U as a grid)
  
  u.OutputPotential(dx,dy); // Text file
  
  u.OutputField(dx,dy); // Text file
  
  u.OutputGraph(d,h); // .eps files made by gnuplot
  
  return 0;
}
