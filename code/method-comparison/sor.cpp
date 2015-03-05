// Dan Richardson, 26/2/15, sor.cpp

// compares convergence of successive over-relaxation to analytical solution
// for different number of iterations

// creates data files of:
// - numerical solution at last iteration
// - average absolute convergence vs iterations
// - CPU time vs iterations
// - average absolute difference between analytical and numerical vs iterations

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
float R = 15;	//radius

//get command line arguments
int loops = atoi(argv[1]); 		//number of iterations

int V=1; //define arbitrary plate potential

double omega=2/(1+M_PI/nx); //define optimum relaxation parameter by given formula

//declare matrices for electrostatic potential
//requires second matrix to store previous values
double phi[nx+2][ny+2];
double phi_new[nx+2][ny+2];

std::cout << "Successive over-relaxation\n";
std::cout << "--------------------------\n";
std::cout << "Defining boundaries...";

//define boundary conditions
for (i=0; i<=nx+1; i++)
{
 for (j=0; j<=ny+1; j++)
 {
  if (j==0 || j == ny+1)
  {
	//linearly decreasing at top and bottom
   	phi[i][j] = V-((double)2*V*i/(nx+1));
   	phi_new[i][j] = phi[i][j];
  }
  else if (i==0)
  {
	//V at left plate
   	phi[i][j] = V;
   	phi_new[i][j] = V;
  }
  else if (i==nx+1)
  {
	//-V at right plate
   	phi[i][j] = -V;
   	phi_new[i][j] = -V;
  }
  else
  {
	//otherwise, initialise to zero
   	phi[i][j] = 0;
   	phi_new[i][j] = 0;
  }
 }
}

std::cout << "Done!\n";

//declare convergence
double convergence = 0;

std::cout << "Solving for potential...";

//iterate to find potential
for (count=0; count < loops; count++)
{
//set to zero at start of each loop
convergence = 0;

 //loop over grid, ignore potential at boundaries (first and last x,y)
 for (i=1; i<=nx; i++)
 {
  for (j=1; j<=ny; j++)
  {
	//if point in circle, potential is 0
   	if ( pow((i*dx-0.5*d),2)+pow((j*dy-0.5*h),2) < pow(R,2) )
   	{
    		phi_new[i][j] = 0;
   	}
   	else
   	{
    		//employ successive over-relaxation
 	   	phi_new[i][j] = (1-omega) * phi[i][j] + omega/4 * (phi_new[i-1][j] + phi[i+1][j] + phi_new[i][j-1] + phi[i][j+1]);
   	}
  }
 }

 //update all of matrix in second loop
 for (i=1; i<=nx; i++)
 {
  for (j=1; j<=ny; j++)
  {
	//increment convergence
	convergence += fabs(phi_new[i][j]-phi[i][j]);

   	phi[i][j] = phi_new[i][j];
  }
 }
}

std::cout << "Done!\n";

double r, theta; //declare polar co-ordinates

std::cout << "Finding differences and writing files...";

double error = 0; //declare difference of analytical and numerical

//output difference of numerical and analytical solution, for a given method
for (j=0; j<=ny+1; j++)
{
 for (i=0; i<=nx+1; i++)
 {
  r=sqrt((i*dx-0.5*d)*(i*dx-0.5*d)+(j*dy-0.5*h)*(j*dy-0.5*h));	//define the radius
  theta = atan2(j*dy-0.5*ny,i*dx-0.5*nx);			//define the angle

  //open file
  std::ofstream file("data/sor.dat", std::ios_base::app);

  //if in the circle, difference will be zero
  if (r*r < R*R)
  {
	//output numerical
   	file << i*dx << "\t" << j*dy << "\t" << 0 << std::endl;
  }
  else //otherwise
  {
	//output numerical
   	file << i*dx << "\t" << j*dy << "\t" << phi[i][j] << std::endl;

	//increment difference
	error += fabs(phi[i][j]-((double)2*V/d)*((double)(R*R)/r - r)*cos(theta));
  }
 }
}

std::cout << "Done!\n";

//end clock
clock_t t1 = clock();

//open files for appending
std::ofstream file1("data/sor_time.dat", std::ios::out | std::ios::app);
std::ofstream file2("data/sor_conv.dat", std::ios::out | std::ios::app);
std::ofstream file3("data/sor_err.dat", std::ios::out | std::ios::app);

//output statistics
file1 << count << '\t' << double(t1 - t0) / CLOCKS_PER_SEC << std::endl;
file2 << count << '\t' << (double)convergence/(nx*ny) << std::endl;
file3 << count << '\t' << (double)error/((nx+2)*(ny+2)) << std::endl;

std::cout << std::endl;
}
