#include <iostream>
#include <iomanip>
#include <time.h>
#include <fstream>
#include <string>
#include <cstdlib>
#include "bitmap_image.hpp"

using namespace std;

// struct to hold values and bool mask
// for arbitrary systems. Mask is used
// to ignore boundary elements:
struct array_data {
	int rows;
	int columns;
	int req_its;

	float ** prev_values;
    float ** values;
    bool ** mask;
    
    float ** xgrad;
    float ** ygrad;
};

array_data * fdm(char*, float*, float, int, float);
void plot(int, string, string);
array_data * locations(char*, float, float, float, float);
void data_out(array_data**, double);
