#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include "bitmap_image.hpp"
#include "class_Laplace.h"
float d, dx, h, dy;
// Main function
int main(int argc, char *argv[])
{
bitmap_image image(argv[1]);

float desiredconv = atof(argv[6]);
float potential[4] = {atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5])}; // potential at blue pixels
float d = image.width();
float h = image.height();
float dx = 1;
float dy = 1;
unsigned char red,green,blue;

Laplace pixels;
Laplace pixels2;
Laplace u;
Laplace prevu;

if (!image)
{
std::cout << "Unable to open file." << std::endl;
exit(EXIT_FAILURE);
}

pixels.SetArraySize(d,h);
u.SetArraySize(d,h);
prevu.SetArraySize(d,h);
for ( unsigned int p = 0 ; p < d ; p++ ){
        for ( unsigned int q = 0 ; q < h ; q++ ){
// analyse RGB values of pixel at (p,q):
                image.get_pixel(p,q,red,green,blue);
// depending on colour of pixel, set potential to user defined values
// Black pixels:
                if ( red < 10 && blue < 10 && green < 10 ){
                        u.array[p][q] = potential[0];
                        pixels.array[p][q]=1;
                }
// Red pixels:
                else if ( red > blue && red > green ){
                        u.array[p][q] = potential[1];
                        pixels.array[p][q]=1;

                }
// Green pixels:
                else if ( green > blue && green > red ){
                        u.array[p][q] = potential[2];
                        pixels.array[p][q]=1;
                }
// Blue pixels:
                else if ( blue > red && blue > green ){
                        u.array[p][q] = potential[3];
                        pixels.array[p][q]=1;
                }
                else {
                        u.array[p][q] = 0;
                        pixels.array[p][q]=0;
                }
        }
}

int count=0;
int itt=0;
float conv;

while (count<(d)*(h)&&itt<10000000){ // loops through until all the pixels meet the required convergence or exceeds a million itterations.
count=0;
        for ( unsigned int i = 0 ; i < d ; i++ ){
                for ( unsigned int j = 0 ; j < h ; j++ ){
                        if (pixels.array[i][j]==1){
                                count++;
                                continue;
                        }
                        if (i == 0){
                                if (j == 0){
                                        u.array[i][j] = 0.5*(u.array[i+1][j]+u.array[i][j+1]);
                                }
                                else if (j == h - 1){
                                        u.array[i][j] = 0.5*(u.array[i+1][j]+u.array[i][j-1]);
                                }
                                else{
                                        u.array[i][j]=(1/3.0)*(u.array[i+1][j]+u.array[i][j+1]+u.array[i][j-1]);
                                }
                        }
// Right edge and corners:
                        else if (i == d - 1){
                                if (j == 0){
                                        u.array[i][j] = 0.5*(u.array[i-1][j]+u.array[i][j+1]);
                                }
                                else if (j == h - 1){
                                        u.array[i][j] = 0.5*(u.array[i-1][j]+u.array[i][j-1]);
                                }
                                else{
                                        u.array[i][j]=(1/3.0)*(u.array[i-1][j]+u.array[i][j+1]+u.array[i][j-1]);
                                }
                        }
// Top edge:
                        else if (j == 0){
                                u.array[i][j]=(1/3.0)*(u.array[i-1][j]+u.array[i+1][j]+u.array[i][j+1]);
                        }
// Bottom edge:
                        else if (j == (h - 1)){
                                u.array[i][j]=(1/3.0)*(u.array[i-1][j]+u.array[i+1][j]+u.array[i][j-1]);
                        }
// Rest of area:
                        else{
                                u.array[i][j]=0.25*(u.array[i+1][j]+u.array[i][j+1]+u.array[i-1][j]+u.array[i][j-1]);
                        }
                        conv = sqrt((prevu.array[i][j]-u.array[i][j])*(prevu.array[i][j]-u.array[i][j])); // finds modulus of the convergence
                        if (conv < desiredconv){
                                count++;
                        }
                        prevu.array[i][j]=u.array[i][j];
                }
        }
itt++;
}
u.OutputPotential(dx,dy); // Text file
u.OutputField(dx,dy); // Text file
u.OutputGraph(d,h); // .eps files made by gnuplot
return 0;
}
