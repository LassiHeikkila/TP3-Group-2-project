#include <iostream>
#include <cstdlib>
#include "bitmap_image.hpp"
/*************************************************************************************
* 'blackpixelcount.cpp' takes a 800x600 or smaller pixel BMP filename as a command   *
* line arguement and counts the number of black pixels as well as constructs a zero  *
* matrix locating the position of each black pixel with a 1.                         *
* The program requires the library 'bitmap_image.hpp'.                               *
* This could easily be turned into a function in the main code and output a pointer  *
* to the matrix. More colours other than black could be used for other areas of      *
* different potentials.                                                              *
*************************************************************************************/

using namespace std;
int main(int argc, char* argv[])
{
bitmap_image image(argv[1]); // opens the image from the commandline arguement
if (!image){
        cout<<"Error - Unable to open: "<<argv[1]<<endl; // error message on failure to open
        return 0;
}
int locate[800][600];
for(int q=0;q<600;q++){
        for(int p=0;p<800;p++){
                locate[p][q]=0; // assigns the matrix 'locate' to have all elements equal to zero
        }
}
int totalblackpixels=0,height=image.height(),width=image.width(); // the height and width of the image in pixels is determined
if (width<800||height<600){
        cout<<"Error - The image is too large"<<endl;
        return 0;
}
unsigned char red,green,blue;
for (int b=0;b<height;b++){
        for(int a=0;a<width;a++){
                image.get_pixel(a,b,red,green,blue); // the quantity of red, blue and green light of the pixel at position (a,b) is analysed
                if(red==0&&blue==0&&green==0){ // if the pixel at (a,b) has no green, blue and red light
                        locate[a][b]=1; // the location of the black pixel is noted in the locate matrix with a 1 
                        totalblackpixels++; // the counter is increased by 1.
                }
        }
}
cout<<"Total black pixels: "<<totalblackpixels<<endl; // the number of black pixels are printed to the screen.
return 0;
}
