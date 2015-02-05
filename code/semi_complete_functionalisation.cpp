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

int dim_x, dim_y, iterations;
float d, dx, h, dy, r;

int BoundaryCylinder(int i, int j);

class Laplace
{
public:
  std::vector< std::vector <float> > array;
  
  // Member function declarations
  void SetArraySize(int dim_x, int dim_y);
  void InitializePotential(float V);
  bool Circle(int i, int j, float r, float dx, float dy, float d, float h);
  void ViewArray();
  void OutputField(float dx, float dy);
  void OutputPotential(float dx, float dy);
  void OutputGraph();
  
};

// Member function definitions
void Laplace::SetArraySize(int dim_x, int dim_y)
// creates an array (made up of vectors of vectors) of size dim_x-by-dim_y
{
  for ( int i = 0 ; i < dim_y ; i++ )
    {
      array.push_back(std::vector<float>()); // Create dim_x empty rows to start with
      // std::cout << "1" << std::endl; // uncomment to see progress for debug purposes
    }
  
  for ( int i = 0 ; i < dim_y ; i++ )
    {
      // std::cout << "2" << std::endl; // uncomment to see progress for debug purposes
      for ( int j = 0 ; j < dim_x ; j++ )
	      {
	        // std::cout << "3" << std::endl; // uncomment to see progress for debugging
	        array[i].push_back(i * j); // create a new vector for each vector
	        array[i][j] = 0; // initialize to 0
	      }
    }
}

void Laplace::InitializePotential(float V) // Works!, 
// initializes array to a linearly decreasing potential from one plate at +V to the other at -V
{
  for ( unsigned int i = 0 ; i < array.size() ; i++ )
    {
      for ( unsigned int j = 0 ; j < array[i].size() ; j++ )
	{
	  if (j==0 || j==array[i].size()) { array[i][j] = V*(1-((float) 2*i/(array.size()-1))); }     //linear potential at top and bottom boundaries
  	  else if (i==0) { array[i][j]=V; }                                         		      //plate is at V
  	  else if (i==array.size()){ array[i][j]=-V; }                              		      //plate is at -V
  	  else { array[i][j] = 0; }                                                 		      //all else 0
	}
    }
}

bool Laplace::Circle(int i, int j, float r, float dx, float dy, float d, float h) 
// returns true if point (i,j) is inside circle of radius r
// returns false otherwise
{
  if ( (pow(d/2 - i*dx,2) + pow(h/2 - j*dy,2)) <= pow(r,2))
    {
      return true;
    }
  else
    {
      return false;
    } 
}

void Laplace::ViewArray() // Works!
// outputs array to terminal 
{
  std::cout << "This is what the array looks like: " << std::endl;
  for ( unsigned int i = 0 ; i < array.size() ; i++ )
    {
      for ( unsigned int j = 0 ; j < array[i].size() ; j++ )
	{
	  std::cout << "\t" << array[i][j];
	}
      std::cout << "\t" << std::endl;
    }
}

void Laplace::OutputField(float dx, float dy) // Works! 
// outputs array to a file called "output.dat"
{
  std::ofstream myfile;
  myfile.open("field.dat");
  // std::cout << "File is open" << std::endl;
  if (myfile.is_open())
    {
      //	myfile << "test1"; // uncomment for debugging
      for ( unsigned int i = 0 ; i < array.size()-1 ; i++ )
	{
	  for ( unsigned int j = 0 ; j < array[i].size()-1 ; j++ )
	    {
	      myfile << i*dx << "\t" << j*dy << "\t" << -(array[i+1][j]-array[i][j])/dx << "\t" << -(array[i][j+1]-array[i][j])/dy << std::endl;
	    }
	  myfile << std::endl;
	}
      //	myfile << "test2"; // uncomment for debugging
      myfile.close();
      // std::cout << "File is closed" << std::endl;
    }
  else 
    {
      std::cout << "I couldn't open the file 'field.dat', so I will exit the program now." << std::endl;
      exit(EXIT_FAILURE);
    }
}

void Laplace::OutputPotential(float dx, float dy)
{
  std::ofstream myfile;
  myfile.open("potential.dat");
  if (myfile.is_open())
    {
      for ( unsigned int i = 0 ; i < array.size() ; i++ )
	{
	  for ( unsigned int j = 0 ; j < array[i].size() ; j++ )
	    {
	      myfile << i*dx << "\t" << j*dy << "\t" << array[i][j] << std::endl;
	    }
	  myfile << std::endl;
	}
      myfile.close();
    }
  else
    {
      std::cout << "I couldn't open potential.dat, so I will exit the program now." << std::endl;
      exit(EXIT_FAILURE);
    }
}

void Laplace::OutputGraph()
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


// Main function
int main(int argc, char *argv[])
{

  float d  = atof(argv[1]); // plate separation
  float dx = atof(argv[2]); // step size in x
  float h  = atof(argv[3]); // plate height
  float dy = atof(argv[4]); // step size in y
  float r  = atof(argv[5]); // radius of cylinder
  int iterations = atoi(argv[6]); // number of iterations

  // validate input by checking all given arguments are sensible

  if ( d < 0 || dx < 0 || h < 0 || dy < 0 || dx > d || dy > h || r >= d/2 || r >= h/2 || iterations < 1)
    {
      std::cout << "Invalid input. Exiting now!" << std::endl;
      exit(EXIT_FAILURE);
    }

  dim_x = floor(d/dx);
  dim_y = floor(h/dy);

  // std::cout << "Enter array size (x - columns): " << std::endl;
  // std::cin >> dim_x;
  // std::cout << "Enter array size (y - rows): " << std::endl;
  // std::cin >> dim_y;


  float V = 1;

  Laplace u;
  u.SetArraySize(dim_x,dim_y);
  
  u.InitializePotential(V);

  for ( int count = 0 ; count < iterations ; count++ )
    {
      for ( unsigned int i = 1 ; i < u.array.size() - 1 ; i++ )
	{
	  for ( unsigned int j = 1 ; j < u.array[i].size() - 1 ; j++ )
	    {
	      if ( u.Circle(i,j,r,dx,dy,d,h) == false )
		{
		  // Ignore the section below or uncomment it for debugging
		  /*
		  if ( count == iterations-1 )
		    {
		      std::cout << " u.Circle(i,j) = false, i = " << i << " j = " << j << std::endl;
		    }
		  */
		  u.array[i][j] = 0.25 * (u.array[i-1][j] + u.array[i+1][j] + u.array[i][j-1] + u.array[i][j+1]);
		}
	      else
		{
		  // Ignose section below or uncomment it for debugging
		  /*
		  if ( count == iterations-1 )
		    {
		      std::cout << "u.Circle(i,j) = true, i = " << i << " j = " << j <<std::endl;
		    }
		  */
		  u.array[i][j] = 0;
		}
	    }
	}
    }
  
  
  // u.ViewArray(); // Uncomment this to see a bunch of meaningless garbage on your screen (not actually meaningless, they are the values of U as a grid)
  
  u.OutputPotential(dx,dy); // Text file

  u.OutputField(dx,dy); // Text file

  u.OutputGraph(); // .eps files made by gnuplot
  
  return 0;
}
