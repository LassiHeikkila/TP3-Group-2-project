#include "headers.h"

array_data * locations(char* file, double p_black, double p_red, double p_green, double p_blue)
{
    // Opens specified bitmap image:
    bitmap_image image(file);
    array_data * ar = new array_data;

    // Error handling for failure to open file:
    if (!image)
    {
        std::cout << "Error: Unable to open '" << file << "'.";
        exit(1);
    }

    // Find dimensions of image in pixels:
    int height = image.height(), width = image.width();

    // Set values of global size variable:
    ar -> rows = height;
    ar -> columns = width;

    unsigned char red,green,blue;
    ar -> values = new double*[width];
    ar -> prev_values = new double*[width];

    // Declare boolean mask:
    ar -> mask = new bool*[width];

    for (int p = 0; p < width; p++)
    {
        ar -> values[p] = new double[height];
        ar -> prev_values[p] = new double[height];
        ar -> mask[p] = new bool[height];

        for (int q = 0; q < height; q++)
        {
            // Initialise array entry to zero:
            ar -> values[p][q] = 0;

            // Analyse RGB values of pixel at (p,q):
            image.get_pixel(p,q,red,green,blue);

            // Depending on colour of pixel, set potential
            // to user defined value. Also set corresponding
            // element of bool mask to 1 for boundary pixels:

            // Black pixels:
            if(red < 10 && blue < 10 && green < 10)
            {
                ar -> values[p][q] = p_black;
                ar -> prev_values[p][q] = p_black;
                ar -> mask[p][q] = 1;
                continue;
            }
            // Red pixels:
            if(red > blue && red > green)
            {
                ar -> values[p][q] = p_red;
                ar -> prev_values[p][q] = p_red;
                ar -> mask[p][q] = 1;
                continue;
            }
            // Green pixels:
            if(green > blue && green > red)
            {
                ar -> values[p][q] = p_green;
                ar -> prev_values[p][q] = p_green;
                ar -> mask[p][q] = 1;
                continue;
            }
            // Blue pixels:
            if(blue > red && blue > green)
            {
                ar -> values[p][q] = p_blue;
                ar -> prev_values[p][q] = p_blue;
                ar -> mask[p][q] = 1;
                continue;
            }
            else
            {
                // If not boundary element, set bool to zero.
                // This will mean the element can be overwritten.
                ar -> mask[p][q] = 0;
            }

        }
    }

    return ar;
}