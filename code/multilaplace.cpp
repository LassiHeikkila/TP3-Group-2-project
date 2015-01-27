#include <iostream>
#include <cstdlib>
#include "bitmap_image.hpp"

/*************************************************************************************
* 'multilaplace.cpp' takes a 800x600 or smaller pixel BMP filename as a command      *
* line arguement as well as 4 different values of potentials to assign to different  * 
* colours on the image in order of black, red, green and blue. The program then      *
* uses the finite difference method to fill in the potential within the rest of the  *
* image. It then outputs the matrix in the form of an 800x600 grid of values.        * 
* The program requires the library 'bitmap_image.hpp'.                               *
*                                                                                    *
* It should be general enough to handle any user defined system. Will be improved    *
* by allowing more than four boundary potentials and dynamic resizing of final       *
* matrices using dimensions of input image.                                          *
*                                                                                    *
*************************************************************************************/

using namespace std;

int rows = 0, columns = 0;

float ** locations(char* file, float p_black, float p_red, float p_green, float p_blue)
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

    for (int p=0;p<width;p++)
    {
        locate[p]=new float[height];
        
        for (int q = 0; q < height; q++)
        {
            // Initialise array entry to zero:
            locate[p][q]=0;

            // Analyse RGB values of pixel at (p,q): 
            image.get_pixel(p,q,red,green,blue);

            // Depending on colour of pixel, set potential
            // to user defined value:

            // Black pixels:
            if(red == 0 && blue == 0 && green == 0)
            {
                locate[p][q] = p_black;
            }
            // Red pixels:
            if(red > blue && red > green)
            {
                locate[p][q]=p_red;
            }
            // Green pixels:
            if(green > blue && green > red)
            {
                locate[p][q]=p_green;
            }
            // Blue pixels:
            if(blue > red && blue > green)
            {
                locate[p][q]=p_blue;
            }

        }
    }
    return locate;
}

int main(int argc, char* argv[])
{
    // Take command line arguments as potential boundary values,
    // to be assigned by colour using locations(): 
    float p_black = atof(argv[2]), p_red = atof(argv[3]);
    float p_green = atof(argv[4]), p_blue = atof(argv[5]);

    // Final command line argument is iteration count:
    int iterations=atoi(argv[6]);

    // Use locations() to build initial array with boundaries:
    float**u=locations(argv[1], p_black, p_red, p_green, p_blue);

    // Loop through desired iteration count:
    for (int count = 0; count < iterations; count++)
    {
        // Loop through x coordinates 
        for (int i = 1;i < 799; i++)
        {
            // Loop through y:
            for (int j = 1; j < 599; j++)
            {
                // Finite Difference Method to calculate potential as
                // average of surrounding values:
                u[i][j]=0.25*(u[i+1][j]+u[i][j+1]+u[i-1][j]+u[i][j-1]);
            }
        }
    }

    // Matrices to hold potential gradient
    // components:
    float x_grad[600][800];
    float y_grad[600][800];

    // Loop through final potential matrix, writing data to file.
    // Simulataneously build and write out E-field matrix.
    for (int j=0; j<=599; j++)
    {
        for (int i=0; i<=799; i++)
        {
            // Write potential to file:
            ofstream file(("potential.dat").c_str(), ios::out | ios::app);
            file << setw(10) << i << "\t" << setw(10) << j << "\t" << setw(10) << u[i][j] << endl;

            // Approximation of gradients:
            x_grad[i][j] = (u[i+1][j]-u[i][j]);
            y_grad[i][j] = (u[i][j+1]-u[i][j]);

            //print out x, y, -du/dx, -du/dy in tabular form (components of E field)
            ofstream file(("field.dat").c_str(), ios::out | ios::app);
            file << setw(10) << i << "\t" << setw(10) << j << "\t" << setw(10) << -x_grad[i][j] << "\t" << setw(10) << -y_grad[i][j] << endl;
        }

    }

    // Cleaning up allocated memory:
    for(int b=0;b<800;b++)
    {
        delete[] u[b];
    }

    delete[] u;
    
    // End.
    return 0;
}
