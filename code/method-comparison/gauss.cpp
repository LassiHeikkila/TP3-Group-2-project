// Dan Richardson, 26/2/15, gauss.cpp

// assesses convergence of gauss-seidel to method analytical solution
// command line arguments: rdaius, max_its and desired convergence

// will create quantitative comparison, i.e. average difference between analytical and numerical for each method
// plot this difference as a function of iterations to see convergence of methods

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>

int main(int argc, char *argv[])
{
//start clock
clock_t t0 = clock();

int count = 0;
int i, j; //declare three counters

//choose arbitrary values, not CLIs for simplicity
int d=30;	//plate separation
int h=30;	//plate height
int dx=1;	//step-size in x
int dy=1;	//step-size in y
int nx=30; 	//x points
int ny=30; 	//y points

float R = atof(argv[1]); 	//radius of cylinder
int loops = atoi(argv[2]); 	//number of iterations
float epsilon = atof(argv[3]); 	//desired convergence

int V=1; //define arbitrary plate potential

//declare matrices for electrostatic potential for different algorithms
float gauss[nx+2][ny+2];
float gauss_new[nx+2][ny+2]; //requires second matrix to store previous values

std::cout << "Gauss-Seidel method\n";
std::cout << "-------------------\n";
std::cout << "Defining boundaries...";

//define boundary conditions
for (i=0; i<=nx+1; i++)
{
 for (j=0; j<=ny+1; j++)
 {
  if (j==0 || j == ny+1)
  {
	//linearly decreasing at top and bottom
   	gauss[i][j] = V-((float)2*V*i/(nx+1));
   	gauss_new[i][j] = V-((float)2*V*i/(nx+1));
  }
  else if (i==0)
  {
	//V at left plate
   	gauss[i][j] = V;
   	gauss_new[i][j] = V;
  }
  else if (i==nx+1)
  {
	//-V at right plate
   	gauss[i][j] = -V;
   	gauss_new[i][j] = -V;
  }
  else
  {
	//otherwise, initialise to zero
   	gauss[i][j] = 0;
   	gauss_new[i][j] = 0;
  }
 }
}

std::cout << "Done!\n";

std::cout << "Solving for potential...";

//stores the number of convergent points
int conv_count;

//iterate to find potential until max its
//or every point has converged to required level
while ( conv_count < nx*ny && count <= loops)
{
//set number of convergeny points to zero
conv_count = 0;

 //loop over grid, ignore potential at boundaries (first and last x,y)
 for (i=1; i<=nx; i++)
 {
  for (j=1; j<=ny; j++)
  {
	//if point in circle, potential is 0
   	if ( pow((i*dx-0.5*d),2)+pow((j*dy-0.5*h),2) < pow(R,2) )
   	{
    		gauss_new[i][j] = 0;
   	}
   	else
   	{
    		//employ successive over-relaxation
 	   	gauss_new[i][j] = 0.25 * (gauss_new[i-1][j] + gauss[i+1][j] + gauss_new[i][j-1] + gauss[i][j+1]);
   	}
	//check convergence
	if (sqrt(pow(gauss_new[i][j]-gauss[i][j],2)) < epsilon) { conv_count++; }
  }
 }

 //update all of gauss
 //requires second loop
 for (i=1; i<=nx; i++)
 {
  for (j=1; j<=ny; j++)
  {
   	gauss[i][j] = gauss_new[i][j];
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
  std::ofstream file("data/gauss.dat", std::ios::out | std::ios::app);

  //if in the circle, difference will be zero
  if (r*r < R*R)
  {
   	file << i*dx << "\t" << j*dy << "\t" << 0 << std::endl;
  }
  else //otherwise
  {
   	file << i*dx << "\t" << j*dy << "\t" << gauss[i][j]/*-((float)2*V/d)*((float)(R*R)/r - r)*cos(theta)*/ << std::endl;
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