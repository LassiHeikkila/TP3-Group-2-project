#include <iostream>
#include <cstdlib>
#include "bitmap_image.hpp"
/*************************************************************************************
* 'multilaplace.cpp' takes a 800x600 or smaller pixel BMP filename as a command      *
* line arguement as well as 4 different values of potentials to assign to different  * 
* colours on the image in order of black, red, green and blue. The program then      *
* uses the finite difference method to fill in the potential within the rest of the  *
* image. It then outputs the matrix in the form of an 800x600 grid of values.        *                                    *
* The program requires the library 'bitmap_image.hpp'.                               *                                                        *
*************************************************************************************/

using namespace std;
float ** locations(char* file, float p1, float p2, float p3, float p4)
{
bitmap_image image(file); // opens the image from the commandline arguement
if (!image){
        cout<<"Error - Unable to open: "<<file<<endl; // error message on failure to open
        return 0;
}
int height=image.height(),width=image.width(); // the height and width of the image in pixels is determined
unsigned char red,green,blue;
float** locate = new float*[width];
for (int p=0;p<width;p++){
        locate[p]=new float[height];
        for (int q=0;q<height;q++){
                locate[p][q]=0;
                image.get_pixel(p,q,red,green,blue); // the quantity of red, blue and green light of the pixel at position (a,b) is analysed
                if(red==0&&blue==0&&green==0){ // if the pixel at (a,b) has no green, blue and red light
                        locate[p][q]=p1; // black pixels are given a potential of p1
                }
                if(red>blue&&red>green){ // if the pixel is red
                        locate[p][q]=p2; // the pixel is given a potential of p2
                }
                if(green>blue&&green>red){ // if the pixel is green
                        locate[p][q]=p3; // the pixel is given a potential of p3
                }
                if(blue>red&&blue>green){ // if the pixel is blue
                        locate[p][q]=p4; // the pixel is given a potential of p4
                }

        }
}
return locate;
}
int main(int argc, char* argv[])
{
float p1=atof(argv[2]),p2=atof(argv[3]),p3=atof(argv[4]),p4=atof(argv[5]); // accepts commandline arguements for 4 different potentials
int iterations=atoi(argv[6]); // the final commandline arguement is the number of iterations
float**u=locations(argv[1],p1,p2,p3,p4);
for (int count=0;count<iterations;count++){
        for (int i=1;i<799;i++){
                for (int j=1;j<599;j++){
                        u[i][j]=0.25*(u[i+1][j]+u[i][j+1]+u[i-1][j]+u[i][j-1]); // finite difference method applied to find potenial
                }
        }
}
for(int b=0;b<800;b++){
        for(int a=0;a<600;a++){
                cout<<u[a][b]; // potential values output in a 800x600 display
        }
cout<<endl;
}
for(int b=0;b<800;b++){
        delete[] u[b]; // allocated memory removed
}
delete[] u;
return 0;
}


