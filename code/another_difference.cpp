// COMPILE WITH "-std=c++11" flag or "stoi" and "stof" may not work

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <vector>

int main(int argc, char *argv[])
{
	int columns = atof(argv[1]); // tell program how many rows image/grid has
	int rows = atof(argv[2]); // tell program how many columns image/grid has

	// make a grid of size specified by command line arguments
	std::vector< std::vector <double> > grid;

	for ( int i = 0 ; i < columns ; i ++ )
	{
		grid.push_back(std::vector<double>());
	}

	for (int i = 0; i < columns; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			grid[i].push_back(j);
			grid[i][j] = 0;
		}
	}

	std::string sLine;
	std::ifstream data;
	data.open ("data/potential.dat");
	if ( data.is_open() )
	{
		std::string x;
		std::string y;
		std::string value;

		std::cout << "Data being read in..." << std::endl;
		while ( std::getline(data,sLine) )
		{
			std::stringstream data(sLine);
			data >> x >> y >> value;

			if ( !sLine.empty() )
			{
			//	std::string::size_type sz;
				int column = stoi(x);
				int row = stoi(y);
				double potential = stod(value);

				grid[column][row] = potential;
			//	std::cout << "X: " << column << "Y: " << row << " grid[x][y]" << grid[column][row] << std::endl;
			}
			else
			{
				continue;
			//	std::cout << "empty string" << std::endl;
			}
			// std::cout << "Working..." << std::endl;
		}
		data.close();
		std::cout << "Data reading finished." << std::endl;
	}
	else 
	{
		std::cout << "Unable to open potential.dat, exiting now." << std::endl;
		exit(EXIT_FAILURE);
	}
int d = rows/2; //half-distance between plates
int h = columns/2; //approximate infinite plate as being half this height
int R = 25; //radius of cylinder
int V = 1; //voltage at plate

double r; //radial co-ordinate
double theta; //angular co-ordinate
double phi = 0; //potential

std::ofstream output;
output.open("data/difference.dat");
std::cout << "Calculating difference and outputting results now..." << std::endl;

for (int x = 0; x < columns ; x++)
	{ //loop over x co-ordinates
//		if ( x % 20 == 0)
//		{
//			std::cout << "x = " << x << std::endl;
//		}
		for (int y = 0; y < rows ; y++)
		{ //loop over y co-ordinates
			r = sqrt((x-d)*(x-d)+(y-h)*(y-h)); // calculate radius from middle at (x,y)
			theta = atan2((y-h),(x-d)); // calculate angle of vector from middle to (x,y)
			
			if ( r*r <= R*R ) //if r^2 < R^2, phi=0
			{
				phi = 0; 
			} //if r^2 < R^2, phi=0
			else  //otherwise, for r > R, phi = (V/d)((R^2/r -r)cos(theta)
			{ 
				phi = ( (double) V/d ) * ( (double) (R*R)/r - r ) * cos(theta); 
			}
			grid[x][y] = grid[x][y] - phi; 
		//	std::cout << "phi = " << phi << " grid[x][y] = " << grid[x][y] << std::endl;
			output << x << "\t" << y << "\t" << grid[x][y] << std::endl;
		}
		output << std::endl;
	}
	output.close();
	std::cout << "Difference calculated and results outputted." << std::endl;
	return 0;
}
