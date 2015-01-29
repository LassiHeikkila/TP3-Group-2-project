#include <iostream>
#include <cstdlib>
#include <cmath>

/****************************************************************
* laplace.cpp 22/1/15                                           *
*                                                               *
* This program numerically solves Laplace's equation            *
*                 Del^2 E = 0                                   *
* where E is the electric field, for the case of an             *
* infinitely long cylinder placed between two infinite plates   *
* of respective voltage -V and +V.                              *
* We find the electrostatic potential around a cross-section    *
* this cylinder: a two-dimensional problem.                     *
*                                                               *
* program is called as: ./exe d dx h dy r iterations            *
* d - plate separation    h - plate height                      *
* dx - step-size in x     dy - step-size in y                   *
* r - radius of cylinder  iterations - no. of iterations        *
*                                                               *
* TO DO:                                                        *
*  - functionalise code                                         *
*  - have plate voltage V and W specified as CLIs               *
*  - define some error tolerance for the relaxation method      *
*  - write bash scipt to run program with CLI                   *
*  - write gnuplot script to plot potential, equipotential      *
*     lines and E field                                         *
*  - fix dynamic memory allocation                              *
****************************************************************/

int main(int argc, char *argv[])
{
int count, i, j; //declare three counters

float d=atof(argv[1]);          //plate separation
float dx=atof(argv[2]);         //x step-size
float h=atof(argv[3]);          //supposed plate height
float dy=atof(argv[4]);         //y step-size
float r = atof(argv[5]);        //radius
int iterations = atoi(argv[6]); //number of iterations

/*
validate input
check dimensions, step-sizes and iterations are positive
and that step-size does not exceed dimensions
and that the cylinder fits in between the plates
*/
if (d < 0 || dx < 0 || h < 0 || dy < 0 || dx > d || dy > h || r >= d || r >= h || iterations < 1)
{
 std::cout << "Invalid input. Exiting momentarily."<< std::endl;
 exit(EXIT_FAILURE);
}

int nx = floor(d/dx);   //number of points in x OR no. of columns
int ny = floor(h/dy);   //number of points in y OR no. of rows

/****************************************************************
//dynamically allocate memory for a 2d array of size nx+1 by ny+1
float** u = new float*[ny+1];

for (i = 0; i < ny+1; ++i)
{
   u[i] = new float[nx+1];
}
                                Still trying to get working
//delete allocated memory
for (i = 0; i < ny+1; ++i)
{
  delete [] u[i];
}

delete [] u;
****************************************************************/

float u[nx+1][ny+1];    //declare matrix for electrostatic potential

int V=1;        //define arbitrary plate potential

//define initial potential linear between plates, from V to -V
for (i=0; i<=nx; i++)
{
 for (j=0; j<=ny; j++)
 {
  u[i][j]=V-((float)2/nx)*V*i;
 }
}

//iterate to find potential, looping over x and y
for (count=0; count < iterations; count++)
{
 for (i=1; i<=nx-1; i++)
 {
  for (j=1; j<=ny-1; j++)
  {
   //if point in circle
   if ((((i*dx)-(0.5*d))*((i*dx)-(0.5*d)))+(((j*dy)-(0.5*h))*((j*dy)-(0.5*h))) < r*r)
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

//print out electrostatic potential, in tabular format
for (j=0; j<=ny; j++)
{
 for (i=0; i<=nx; i++)
 {
  //print out x, y, u
  std::cout << i*dx << "\t" << j*dy << "\t" << u[i][j] << '\n';
 }
 std::cout << std::endl;
}

float x_grad[nx][ny];   //matrices to hold gradient of potential
float y_grad[nx][ny];

//find gradient of electrostatic potential in x and y directions
for (j=0; j<=ny-1; j++)
{
 for (i=0; i<=nx-1; i++)
 {
  x_grad[i][j] = (u[i+1][j]-u[i][j])/dx;        //approximate gradients
  y_grad[i][j] = (u[i][j+1]-u[i][j])/dy;

  //print out x, y, -du/dx, -du/dy in tabular form (components of E field)
  std::cout << i*dx << "\t" << j*dy << "\t" << -x_grad[i][j] << "\t" << -y_grad[i][j] << '\n';
 }
 std::cout << std::endl;
}

}
