#include <iostream>
#include <iomanip>
#include <time.h>
#include <fstream>
#include <string>
#include <string.h>
#include <stdio.h>
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

	double ** prev_values;
    double ** values;
    bool ** mask;
    
    double ** xgrad;
    double ** ygrad;
};

array_data * fdm(char*, double*, double, int, double);
void plot(int, string, string, int, int);
array_data * locations(char*, double, double, double, double);
void data_out(array_data**, double);
int parseline(char*);
int mem_measure();
bool mintrue(array_data*,int,int, int);
