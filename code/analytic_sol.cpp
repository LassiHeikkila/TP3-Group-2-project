//function outputs analytical solution of laplace equation in polar co-ordinates
//for the electrostatic system A

#include <iostream>
#include <cmath>

int main()
{
int d = 100;    //half-distance between plates
int h = 100;    //approximate infinite plate as being half this height
int R = 25;     //radius of cylinder
int V = 1;      //voltage at plate
float r;        //radial co-ordinate
float theta;    //angular co-ordinate
float phi = 0;  //potential

for (int x=-d; x<=d; x++){                                      //loop over x co-ordinates
 for (int y=-h; y<=h; y++){                                     //loop over y co-ordinates
  r = sqrt(x*x+y*y), theta = atan2(y,x);                        //define radius and angle
  if(r*r <= R*R) { phi=0; }                                     //if r^2 < R^2, phi=0
  else { phi = ((float)V/d)*((float)(R*R)/r - r)*cos(theta); }  //otherwise, for r > R, phi = (V/d)((R^2/r -r)cos(theta)
  std::cout << x << '\t' << y << '\t' << phi << std::endl;      //output x, y, phi(x,y)
 }
}

} //end of main function

