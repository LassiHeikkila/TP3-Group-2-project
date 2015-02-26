// COMPILE WITH "-std=c++11" flag or "stoi" and "stof" may not work
// run with ./name columns rows file1 file2


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
	std::string file_1 = argv[3];
	std::string file_2 = argv[4];
	// make a grid of size specified by command line arguments
	std::vector< std::vector <double> > grid_a;
	std::vector< std::vector <double> > grid_b;
	std::vector< std::vector <double> > grid_diff;

	for ( int i = 0 ; i < columns ; i ++ )
	{
		grid_a.push_back(std::vector<double>());
		grid_b.push_back(std::vector<double>());
		grid_diff.push_back(std::vector<double>());
	}
	for (int i = 0; i < columns; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			grid_a[i].push_back(j);
			grid_b[i].push_back(j);
			grid_diff[i].push_back(j);
			grid_a[i][j] = 0;
			grid_b[i][j] = 0;
			grid_diff[i][j] = 1;
		}
	}
	std::string sLine;
	std::ifstream data;

// FILE_1

	data.open (("data/"+file_1).c_str());
	if ( data.is_open() )
	{
		std::string x;
		std::string y;
		std::string value;

		std::cout << ("Data being read in from "+file_1).c_str() << "..." << std::endl;
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

				grid_a[column][row] = potential;
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
		std::cout << ("Unable to open "+file_1+", exiting now.").c_str() << std::endl;
		exit(EXIT_FAILURE);
	}

// FILE_2

		data.open (("data/"+file_2).c_str());
	if ( data.is_open() )
	{
		std::string x;
		std::string y;
		std::string value;

		std::cout << ("Data being read in from "+file_2).c_str() << "..." << std::endl;
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

				grid_b[column][row] = potential;
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
		std::cout << ("Unable to open "+file_2+", exiting now.").c_str() << std::endl;
		exit(EXIT_FAILURE);
	}

std::ofstream output;
output.open("data/difference.dat");
std::cout << "Calculating difference and outputting results now..." << std::endl;

double epsilon = 0.000000001;

for (int x = 0; x < columns ; x++)
	{ //loop over x co-ordinates
//		if ( x % 20 == 0)
//		{
//			std::cout << "x = " << x << std::endl;
//		}
		for (int y = 0; y < rows ; y++)
		{ //loop over y co-ordinates
			if ( grid_a[x][y] <= epsilon && grid_b[x][y] <= epsilon )
			{
				grid_diff[x][y] = 0;
			}
			else
			{
				grid_diff[x][y] = std::abs(2 * (grid_a[x][y] - grid_b[x][y]) / (grid_a[x][y]+grid_b[x][y]))*100;	
			}
			output << x << "\t" << y << "\t" << grid_diff[x][y] << std::endl;
		}
		output << std::endl;
	}
	output.close();
	std::cout << "Difference calculated and results outputted." << std::endl;
	return 0;
}
