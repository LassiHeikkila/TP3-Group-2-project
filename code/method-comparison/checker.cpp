// Dan Richardson, 26/2/15, checker.cpp

// assesses convergence of checkerboard updating to analytical solution
// command line arguments: radius, max_its and desired convergence

// will create quantitative comparison, i.e. average difference between analytical and numerical for each method
// plot this difference as a function of iterations to see convergence of methods
// will implement relative convergence

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>

int main(int argc, char *argv[])
{
//start clock
clock_t t0 = clock();

//declare three counters
int i = 0, j = 0, count = 0;

//arbitrary values, not CLIs for simplicity
int d = 100;	//plate separation
int h = 100;	//plate height
int dx = 1;	//step-size in x
int dy = 1;	//step-size in y
int nx = 100; 	//x points
int ny = 100; 	//y points

//get command line arguments
float R = atof(argv[1]); 	//radius of cylinder
int loops = atoi(argv[2]); 	//number of iterations
float epsilon = atof(argv[3]); 	//desired convergence

int V=1; //define arbitrary plate potential

float omega=2/(1+float(M_PI)/nx); //define optimum relaxation parameter by given formula

//declare matrices for electrostatic potential
//requires second matrix to store previous values
float checker[nx+2][ny+2];
float checker_new[nx+2][ny+2];

std::cout << "Checkboard (red-black) updating\n";
std::cout << "-------------------------------\n";
std::cout << "Defining boundaries...";

//define boundary conditions
for (i=0; i<=nx+1; i++)
{
 for (j=0; j<=ny+1; j++)
 {
  if (j==0 || j == ny+1)
  {
	//linearly decreasing at top and bottom
   	checker[i][j] = V-((float)2*V*i/(nx+1));;
   	checker_new[i][j] = V-((float)2*V*i/(nx+1));;
  }
  else if (i==0)
  {
	//V at left plate
   	checker[i][j] = V;
   	checker_new[i][j] = V;
  }
  else if (i==nx+1)
  {
	//-V at right plate
   	checker[i][j] = -V;
   	checker_new[i][j] = -V;
  }
  else
  {
	//otherwise, initialise to zero
   	checker[i][j] = 0;
   	checker_new[i][j] = 0;
  }
 }
}

std::cout << "Done!\n";

std::cout << "Solving for potential...";

//stores the number of convergent points
int conv_count;

//iterate to find potential until max its
//or every point has converged to required level
while (conv_count < nx*ny && count <= loops)
{
//set number of convergent points to zero
conv_count = 0;

 //loop over grid, ignore potential at boundaries (first and last x,y)
 for (i=1; i<=nx; i++)
 {
  for (j=1; j<=ny; j++)
  {
	//if point in circle, potential is 0
   	if ( pow((i*dx-0.5*d),2)+pow((j*dy-0.5*h),2) < pow(R,2) )
   	{
    		checker_new[i][j] = 0;
   	}
   	else
   	{
 	   	//employ checkerboard updating, for even points
    		if ((i + j) % 2 == 0)
    		{
     			checker_new[i][j] = (1-omega) * checker[i][j] + omega/4 * (checker[i-1][j] + checker[i+1][j] + checker[i][j-1] + checker[i][j+1]);
    		}
   	}
  }
 }

 //update matrix at odd points in second loop
 for (i=1; i<=nx; i++)
 {
  for (j=1; j<=ny; j++)
  {
	//odd points
   	if ((i + j) % 2 != 0)
   	{
		checker_new[i][j] = (1-omega) * checker[i][j] + omega/4 * (checker_new[i-1][j] + checker_new[i+1][j] + checker_new[i][j-1] + checker_new[i][j+1]);
   	}
  }
 }

 //update all of checker and check convergence in third loop
 for (i=1; i<=nx; i++)
 {
  for (j=1; j<=ny; j++)
  {
	//check convergence
	if (fabs(checker_new[i][j]-checker[i][j]) < epsilon) { conv_count++; }

   	checker[i][j] = checker_new[i][j];
  }
 }

//increment the counter
count++;
}

std::cout << "Done!\n";

float r, theta; //declare polar co-ordinates

std::cout << "Finding differences and writing files...";

//output difference of numerical and analytical solution, for a given method
for (j=0; j<=ny+1; j++)
{
 for (i=0; i<=nx+1; i++)
 {
  r=sqrt((i*dx-0.5*d)*(i*dx-0.5*d)+(j*dy-0.5*h)*(j*dy-0.5*h));	//define the radius
  theta = atan2(j*dy-0.5*ny,i*dx-0.5*nx);			//define the angle

  //open files for appending
  std::ofstream file("data/checker.dat", std::ios::out | std::ios::app);

  //if in the circle, difference will be zero
  if (r*r < R*R)
  {
   	file << i*dx << "\t" << j*dy << "\t" << 0 << std::endl;
  }
  else //otherwise
  {
   	file << i*dx << "\t" << j*dy << "\t" << checker[i][j]/*-((float)2*V/d)*((float)(R*R)/r - r)*cos(theta)*/ << std::endl;
  }
 }
}

std::cout << "Done!\n";

//end clock
clock_t t1 = clock();

//output statistics
std::cout << "Convergence: " << epsilon << std::endl;
std::cout << "Iterations: " << count << std::endl;
std::cout << "CPU time: " << double(t1 - t0) / CLOCKS_PER_SEC << "s" << std::endl;
std::cout << std::endl;
}
