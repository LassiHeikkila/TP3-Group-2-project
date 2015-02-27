// Dan Richardson, 26/2/15

// rudimentary program to compare difference between
// analytical solution and various numerical methods:
// - jacobi's iterative method
// - gauss-seidel method
// - successive over-relaxation
// - checkerboard (red-black) updating  <--- THIS METHOD DOES NOT WORK YET, or at least converges rather slowly

// only graphical comparison so far
// will implement convergence condition
// will create quantitative comparison, i.e. average difference between analytical and numerical for each method
// plot this difference as a function of iterations to see convergence of methods

//NOTE: seg faults for larger matrices, probably due to having to create six, at least, of them

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>

int main(int argc, char *argv[])
{
//start clock
clock_t t0 = clock();

int count, i, j; //declare three counters

//choose arbitrary values, not CLIs for simplicity
int d=30;	//plate separation
int h=30;	//plate height
int dx=1;	//step-size in x
int dy=1;	//step-size in y
int nx=30; 	//x points
int ny=30; 	//y points

float R = atof(argv[1]); 	//radius of cylinder
int loops = atoi(argv[2]); 	//number of iterations

int V=1; //define arbitrary plate potential

float omega=2/(1+float(M_PI)/nx); //define optimum relaxation parameter by given formula

//declare matrices for electrostatic potential for different algorithms
float jacobi[nx+2][ny+2];
float jacobi_new[nx+2][ny+2]; //requires second matrix to store previous values

float gauss[nx+2][ny+2];
float sor[nx+2][ny+2];

float checker[nx+2][ny+2];
float checker_new[nx+2][ny+2]; //requires second matrix to store previous values

std::cout << "Defining boundaries...";

//define boundary conditions
for (i=0; i<=nx+1; i++)
{
 for (j=0; j<=ny+1; j++)
 {
  if (j==0 || j == ny+1)
  {
	//linearly decreasing at top and bottom
   	jacobi[i][j] = V-((float)2*V*i/(nx+1));
   	gauss[i][j] = V-((float)2*V*i/(nx+1));
   	sor[i][j] = V-((float)2*V*i/(nx+1));
   	checker[i][j] = V-((float)2*V*i/(nx+1));;
  }
  else if (i==0)
  {
	//V at left plate
   	jacobi[i][j] = V;
   	gauss[i][j] = V;
   	sor[i][j] = V;
   	checker[i][j] = V;
  }
  else if (i==nx+1)
  {
	//-V at right plate
   	jacobi[i][j] = -V;
   	gauss[i][j] = -V;
   	sor[i][j] = -V;
   	checker[i][j] = -V;
  }
  else
  {
	//otherwise, initialise to zero
   	jacobi[i][j] = 0;
   	gauss[i][j] = 0;
   	sor[i][j] = 0;
   	checker[i][j] = 0;
  }
 }
}

std::cout << "Done!\n";

std::cout << "Solving for potential...\n";

//iterate to find potential, looping over x and y
//ignore potential at boundaries (first and last x,y)
for (count=0; count <= loops; count++)
{
 for (i=1; i<=nx; i++)
 {
  for (j=1; j<=ny; j++)
  {
	//if point in circle, potential is 0
   	if ( pow((i*dx-0.5*d),2)+pow((j*dy-0.5*h),2) < pow(R,2) )
   	{
		jacobi_new[i][j] = 0;
   		gauss[i][j] = 0;
    		sor[i][j] = 0;
    		checker_new[i][j] = 0;
   	}
   	else
   	{
    		//employ jacobi's method
    		jacobi_new[i][j] = 0.25 * (jacobi[i-1][j] + jacobi[i+1][j] + jacobi[i][j-1] + jacobi[i][j+1]);

    		//employ gauss-seidel method
    		gauss[i][j] = 0.25 * (gauss[i-1][j] + gauss[i+1][j] + gauss[i][j-1] + gauss[i][j+1]);

    		//employ successive over-relaxation
 	   	sor[i][j] = (1-omega) * sor[i][j] + omega/4 * (sor[i-1][j] + sor[i+1][j] + sor[i][j-1] + sor[i][j+1]);

 	   	//employ checkerboard updating, for even points
    		if ((i + j) % 2 == 0)
    		{
     			checker_new[i][j] = (1-omega) * checker[i][j] + omega/4 * (checker[i-1][j] + checker[i+1][j] + checker[i][j-1] + checker[i][j+1]);
    		}
   	}
  }
 }

 //update checker at odd points
 //requires second loop
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

 //update all of jacobi and checker
 //requires second loop
 for (i=1; i<=nx; i++)
 {
  for (j=1; j<=ny; j++)
  {
   	jacobi[i][j] = jacobi_new[i][j];
   	checker[i][j] = checker_new[i][j];
  }
 }

//display every 1000 iterations
if (count % 1000 == 0) { std::cout << "\rIteration " << count << "." << std::flush; }
}

std::cout << std::endl;
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
  std::ofstream jacobi_file("data/jacobi.dat", std::ios::out | std::ios::app);
  std::ofstream gauss_file("data/gauss.dat", std::ios::out | std::ios::app);
  std::ofstream sor_file("data/sor.dat", std::ios::out | std::ios::app);
  std::ofstream checker_file("data/checker.dat", std::ios::out | std::ios::app);

  //if in the circle, difference will be zero
  if (r*r < R*R)
  {
   	jacobi_file << i*dx << "\t" << j*dy << "\t" << 0 << std::endl;
   	gauss_file << i*dx << "\t" << j*dy << "\t" << 0 << std::endl;
   	sor_file << i*dx << "\t" << j*dy << "\t" << 0 << std::endl;
   	checker_file << i*dx << "\t" << j*dy << "\t" << 0 << std::endl;
  }
  else //otherwise
  {
   	jacobi_file << i*dx << "\t" << j*dy << "\t" << jacobi[i][j]-((float)2*V/d)*((float)(R*R)/r - r)*cos(theta) << std::endl;
   	gauss_file << i*dx << "\t" << j*dy << "\t" << gauss[i][j]-((float)2*V/d)*((float)(R*R)/r - r)*cos(theta) << std::endl;
   	sor_file << i*dx << "\t" << j*dy << "\t" << sor[i][j]-((float)2*V/d)*((float)(R*R)/r - r)*cos(theta) << std::endl;
   	checker_file << i*dx << "\t" << j*dy << "\t" << checker[i][j]/*-((float)2*V/d)*((float)(R*R)/r - r)*cos(theta)*/ << std::endl;
  }
 }
}

std::cout << "Done!\n";

//end clock
clock_t t1 = clock();
std::cout << "CPU time = " << double(t1 - t0) / CLOCKS_PER_SEC << " sec" << std::endl;
}
