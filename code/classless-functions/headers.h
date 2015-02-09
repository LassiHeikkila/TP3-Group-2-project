#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "bitmap_image.hpp"

// struct to hold values and bool mask
// for arbitrary systems. Mask is used
// to ignore boundary elements:
struct array_data {
	int rows;
	int columns;

  float ** values;
  bool ** mask;
  
  float ** xgrad;
  float ** ygrad;
};
