#include <iostream>
#include <cstdlib>
#include <fstream>
#include "bitmap_image.hpp"
/*************************************************************************************
* 'multilaplace.cpp' takes a BMP filename as a command line arguement as well as 3   *
* different values of potentials to assign to different colours on the image in order*
* of black, red and green. A linear function in either the x or y direction can also *
* formed using a blue line and requires values of y intersect and gradient as well as*
* a switch of 0 or 1 (0 for x direction, 1 for y direction) from the command line.   *
* The program then uses the finite difference method to fill in the potential within *
* the rest of the image. The gradient is then found between points and the results   *
* are all sent to the data files 'potential.dat' and 'field.dat'.                    *
* The program requires the library 'bitmap_image.hpp'.                               *
* Some potential improvements:                                                               *
*  - The speed of the program is slow. Mess of 'if' statements could potentially be  *
*  simplified in the loop.                                                           *
*  - The extreme boundaries could be solved with additional conditions.              *
*  - GNU plot could be automatically called up at the end..                          *
*  - Additional functions could be added and called upon in the commandline.         *
*  - More importantly MORE EFFICIENT ALGORITHM could replace the finite difference.  *
*  - A clever simulation of infinite boundaries? Maybe use values on one side without*
*  colour to determine values on the other side?                                     *
*  - Alter to find a desired rate of convergence instead of iterations.              *
*  - Errors etc.                                                                     *
*************************************************************************************/
using namespace std;
int rows = 0, columns = 0;

float linear(int x, float m, float c) //Function in the form of a 'straight line' equation 
{
float out=(m*x)+c; // the output uses the gradient 'm' and intersect 'c' in the form 'y = m x + c'.
return out;
}

float ** locations(char* file) // used to locate the positions and types of boundary conditions.
{
// Opens specified bitmap image:
bitmap_image image(file);
// Error handling for failure to open file:
if (!image)
{
cout<<"Error - Unable to open: "<<file<<endl;
return 0;
}
// Find dimensions of image in pixels:
int height=image.height(),width=image.width();
// Set values of global size variable:
rows = height;
columns = width;
unsigned char red,green,blue;
float** locate = new float*[width];
for (int p=0;p<width;p++){
        locate[p]=new float[height];
        for (int q = 0; q < height; q++){
// Initialise array entry to zero:
                locate[p][q]=0;
// Analyse RGB values of pixel at (p,q):
                image.get_pixel(p,q,red,green,blue);
// Depending on colour of pixel, set potential
// to user defined value:
// Black pixels:
                if(red < 10 && blue < 10 && green < 10){
                        locate[p][q] = 1;
                }
// Red pixels:
                if(red > blue && red > green){
                        locate[p][q]= 2;
                }
// Green pixels:
                if(green > blue && green > red){
                        locate[p][q]= 3;
                }
// Blue pixels:
                if(blue > red && blue > green){
                        locate[p][q]= 4;
                }
        }
}
return locate;
}
int main(int argc, char* argv[])
{
// Use locations() to build initial array with boundaries:
float**a=locations(argv[1]);
float u[columns][rows];
// Loop through desired iteration count:
for (int count = 0; count < atoi(argv[8]); count++){
// Loop through x coordinates
        for (int i=0;i < columns; i++){
// Loop through y:
                for (int j=0; j < rows; j++){
// Finite Difference Method to calculate potential as
// average of surrounding values:
                        if(count==0){ //initializes the matrix u in the first iteration
                                u[i][j]=0;
                        }
                        else{
                                if(a[i][j]==0){
                                        if(i>0 && i<(columns-1) && j>0 && j<(rows-1)){ // if the value is not a boundary or extremity:
                                                u[i][j]=0.25*(u[i+1][j]+u[i][j+1]+u[i-1][j]+u[i][j-1]); 
// finite difference method is used to find the missing value.
                                        }
                                }
                                else{ // for boundaries:
                                        if(a[i][j]==1){ // if a black pixel:
                                                u[i][j]=atof(argv[2]); // 2nd command line argument assigned.
                                        }
                                        if(a[i][j]==2){ // if a red pixel:
                                                u[i][j]=atof(argv[3]); // 3rd command line argument assigned.
                                        }
                                        if(a[i][j]==3){ // if a green pixel:
                                                u[i][j]=atof(argv[4]); // 4th command line argument assigned.
                                        }
                                        if(a[i][j]==4){ // if a blue pixel:
                                                if(atoi(argv[7])==0){ // if argument 5 is 0 (indicates relation in x direction):
                                                        u[i][j]=linear(i,atof(argv[5]),atof(argv[6])); 
// argument 5 and 6 used with linear function as well as the column value.
                                                }
                                                if(atoi(argv[7])==1){ // if argument 5 is 1 (indicates relation in y direction):
                                                        u[i][j]=linear(j,atof(argv[5]),atof(argv[6]));
// argument 5 and 6 used with linear function as well as the row value.
                                                }
                                        }
                                }
                        }
                }
        }
}
// Matrices to hold potential gradient
// components:
float x_grad[columns][rows];
float y_grad[columns][rows];
// Loop through final potential matrix, writing data to file.
// Simulataneously build and write out E-field matrix.
for (int j=0; j<rows; j++){
        for (int i=0; i<columns; i++){
// Write potential to file:
                ofstream file("potential.dat", ios::out | ios::app);
        file << i << "\t" << j << "\t" << u[i][j] << endl;
// Approximation of gradients:
                x_grad[i][j] = (u[i+1][j]-u[i][j]);
                y_grad[i][j] = (u[i][j+1]-u[i][j]);
//print out x, y, -du/dx, -du/dy in tabular form (components of E field)
                ofstream file2("field.dat", ios::out | ios::app);
                file2 << i << "\t" << j << "\t" << -x_grad[i][j] << "\t" << -y_grad[i][j] << endl;
        }
}
// Cleaning up allocated memory:
for(int b=0;b<columns;b++){
        delete[] a[b];
}
delete[] a;
// End.
return 0;
}
