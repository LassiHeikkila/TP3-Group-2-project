#include <iostream>
#include <iomanip>
#include <time.h>
#include <fstream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include <thread>		// thread::hardware_concurrency() - Counts available cores.
//#include <pthread.h>	// Thread creation etc.
#include <boost/threadpool.hpp>
#include <boost/bind.hpp>
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

struct input_args {
	array_data* sys;
	double relaxation;
	double convergence;
	int iterations;
	int* convcounts;

	bool redblack;
	bool lock;
};


// array_data * fdm(array_data*, double, int, double);
void fdm(int,int);
void plot(int, string, string, int, int);
array_data * locations(const char*, double, double, double, double);
void data_out(array_data**, double);
int parseline(char*);
int mem_measure();
bool mintrue(array_data*,int,int, int);
void threader(int);
timespec diff(timespec, timespec);

extern input_args* input;
extern bool startbool;

