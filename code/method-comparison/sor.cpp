// Dan Richardson, 26/2/15, sor.cpp

// compares difference between analytical solution and successive over-relaxation
// clis: radius, max_its, convergence

//creates data files of
// - numerical solution
// - iterations vs desired convergence
// - CPU time vs iterations
// - difference between analytical and numerical vs iterations

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
float R = atof(argv[1]); 		//radius of cylinder
int loops = atoi(argv[2]); 		//number of iterations
double epsilon = atof(argv[3]); 	//desired convergence

int V=1; //define arbitrary plate potential

float omega=2/(1+float(M_PI)/nx); //define optimum relaxation parameter by given formula

//declare matrices for electrostatic potential
//requires second matrix to store previous values
float sor[nx+2][ny+2];
float sor_new[nx+2][ny+2];

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
   	sor[i][j] = V-((float)2*V*i/(nx+1));
   	sor_new[i][j] = V-((float)2*V*i/(nx+1));
  }
  else if (i==0)
  {
	//V at left plate
   	sor[i][j] = V;
   	sor_new[i][j] = V;
  }
  else if (i==nx+1)
  {
	//-V at right plate
   	sor[i][j] = -V;
   	sor_new[i][j] = -V;
  }
  else
  {
	//otherwise, initialise to zero
   	sor[i][j] = 0;
   	sor_new[i][j] = 0;
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
    		sor_new[i][j] = 0;
   	}
   	else
   	{
    		//employ successive over-relaxation
 	   	sor_new[i][j] = (1-omega) * sor[i][j] + omega/4 * (sor_new[i-1][j] + sor[i+1][j] + sor_new[i][j-1] + sor[i][j+1]);
   	}
	//check convergence
	if (fabs(sor_new[i][j]-sor[i][j]) < epsilon) { conv_count++; }
//	if ( fabs(sor_new[i][j]/sor[i][j]) < 1 + epsilon || fabs(sor_new[i][j]/sor[i][j]) > 1 - epsilon ) { conv_count++; }

  }
 }

 //update all of matrix in second loop
 for (i=1; i<=nx; i++)
 {
  for (j=1; j<=ny; j++)
  {
   	sor[i][j] = sor_new[i][j];
  }
 }

//increment the counter
count++;
}

std::cout << "Done!\n";

float r, theta; //declare polar co-ordinates

std::cout << "Finding differences and writing files...";

float difference = 0;

//output difference of numerical and analytical solution, for a given method
for (j=0; j<=ny+1; j++)
{
 for (i=0; i<=nx+1; i++)
 {
  r=sqrt((i*dx-0.5*d)*(i*dx-0.5*d)+(j*dy-0.5*h)*(j*dy-0.5*h));	//define the radius
  theta = atan2(j*dy-0.5*ny,i*dx-0.5*nx);			//define the angle

  //open file
//std::ofstream file("data/sor.dat", std::ios::out);

  //if in the circle, difference will be zero
  if (r*r < R*R)
  {
	//output numerical
//   	file << i*dx << "\t" << j*dy << "\t" << 0 << std::endl;
  }
  else //otherwise
  {
	//output numerical
//   	file << i*dx << "\t" << j*dy << "\t" << sor[i][j] << std::endl;

	//increment difference
	difference += sor[i][j]-((float)2*V/d)*((float)(R*R)/r - r)*cos(theta);
  }
 }
}

std::cout << "Done!\n";

//end clock
clock_t t1 = clock();

//open files for appending
std::ofstream file1("data/sor_its.dat", std::ios::out | std::ios::app);
std::ofstream file2("data/sor_time.dat", std::ios::out | std::ios::app);
std::ofstream file3("data/sor_diff.dat", std::ios::out | std::ios::app);

//output statistics
file1 << epsilon << '\t' << count << std::endl;
file2 << epsilon << '\t' << double(t1 - t0) / CLOCKS_PER_SEC << std::endl;
file3 << count << '\t' << difference/((nx+2)*(ny+2)) << std::endl;

std::cout << std::endl;
}