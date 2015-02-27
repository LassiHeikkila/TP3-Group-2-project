//program plots difference of analytic and numerical solution

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>

int main(int argc, char *argv[])
{
int count, i, j; //declare three counters

float d=atof(argv[1]); //plate separation
float dx=atof(argv[2]); //x step-size
float h=atof(argv[3]); //supposed plate height
float dy=atof(argv[4]); //y step-size
float R = atof(argv[5]); //radius
int loops = atoi(argv[6]); //number of iterations

//validate input
if (d < 0 || dx < 0 || h < 0 || dy < 0 || dx > d || dy > h || R >= d || R >= h || loops < 1)
{
 std::cout << "Invalid input. Exiting momentarily."<< std::endl;
 exit(EXIT_FAILURE);
}

int nx = floor(d/dx); //number of points in x
int ny = floor(h/dy); //number of points in y

float u[nx+1][ny+1]; //declare matrix for electrostatic potential

int V=1; //define arbitrary plate potential

//define boundary conditions
for (i=0; i<=nx; i++)
{
 for (j=0; j<=ny; j++)
 {
  if (j==0 || j == ny) { u[i][j] = V-((float)2*V*i/nx); }	//linear potential at top and bottom boundary
  else if (i==0) { u[i][j]=V; } 				//plate is at V
  else if (i==nx){ u[i][j]=-V; } 				//plate is at -V
  else { u[i][j] = 0; } 					//all else 0
 }
}

//iterate to find potential, looping over x and y
//ignore potential at boundaries (first and last x,y)
for (count=0; count < loops; count++)
{
 for (i=1; i<=nx-1; i++)
 {
  for (j=1; j<=ny-1; j++)
  {
   //if point in circle
   if ((((i*dx)-(0.5*d))*((i*dx)-(0.5*d)))+(((j*dy)-(0.5*h))*((j*dy)-(0.5*h))) < R*R)
   {
    u[i][j] = 0; //potential is 0
   }
   else
   {
    u[i][j]=0.25*(u[i+1][j]+u[i][j+1]+u[i-1][j]+u[i][j-1]); //point is average of four surrounding points
   }
  }
 }
}

float r, theta;

//output difference of numerical and analatyical solution
for (j=-0.5*ny; j<=0.5*ny; j++)
{
 for (i=-0.5*nx; i<=0.5*nx; i++)
 {
  r=sqrt((i*dx)*(i*dx)+(j*dy)*(j*dy)); 	//define the radius
  theta = atan2(j*dy,i*dx);		//define the angle

  int v = 0.5*nx; //define shift in x co-ordinates
  int w = 0.5*ny; //define shift in y co-ordinates

  //if in the circle, difference will be zero
  if ((i*dx)*(i*dx)+(j*dy)*(j*dy) < R*R)
  {
   std::cout << i*dx << "\t" << j*dy << "\t" << 0 << std::endl;
  }
  else
  {
   std::cout << i*dx << "\t" << j*dy << "\t" << u[i+v][j+w]-((float)2*V/d)*((float)(R*R)/r - r)*cos(theta) << std::endl;
  }
 }
}

}
