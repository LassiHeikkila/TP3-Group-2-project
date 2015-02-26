// COMPILE WITH "-std=c++11" flag or "stoi" and "stof" may not work

// This program was written by Lassi Heikkilä, a theoretical physics student @ The University of Glasgow
// The program takes in two files and calculates the difference between each data point in those files.
// 
// The files need to have the following form:
// 	1st column: x coordinate
// 	2nd column: y coordinate
// 	3rd column: value at the (x,y) coordinates
//
// The columns should be separated by tabs, and as required by gnuplot,
//	after each change in x there should be an empty line.
//
// The program takes in command line arguments as follows:
// 0 - program name
// 1 - number of columns in input files (i.e. range of x), (if x goes from 0 to 399 then enter 400, etc.)
// 2 - number of rows in input files (i.e. range of y), (if y goes from 0 to 299 then enter 300, etc.)
// 3 - name of input file 1 (will be in the data/ subdirectory, the directory name does not need to be entered)
// 4 - name of input file 2 -||-, NOTE: enter full name of file with extension (.dat or whatever)
//
// So for example, if the program was compiled as difference.exe,
//	the x range 0 to 399, the y range 0 to 299 and the files located in data/ called file1.dat and file2.dat,
//	then you would execute the program by typing in "./difference.exe 400 300 file1.dat file2.dat".
// 
// The program outputs the results in the same format as the input files
// which can then be plotted in gnuplot, for example with pm3d map and splot.
// The output file is called difference.dat and can be found in the data/ subdirectory.


#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <vector>
#include <ctime>


int main(int argc, char *argv[])
{
	// Create a bunch of variables of type clock_t to be used in recording program execution time
	clock_t tm_initial, tm_misc, tm_read1, tm_read2, tm_calculate_diff, tm_write, tm_total;
	tm_initial = clock(); // record clock time at the beginning of the program

	std::cout << "Creating necessary arrays and initialising program..." << std::endl;

	int columns = atof(argv[1]);	// tell program how many rows image/grid has
	int rows = atof(argv[2]);		// tell program how many columns image/grid has
	std::string file_1 = argv[3];	// read argv[3] as name of file_1
	std::string file_2 = argv[4];	// read argv[4] as name of file_2
	
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

	std::string sLine;	// initialize string called sLine to be used when reading in the input files line by line
	std::ifstream data;	// initialize input file stream called data which will hold the input data in it

	tm_misc = clock() - tm_initial;	// Record how long the initial steps took
	
	std::cout << "Arrays created and program initialized." << std::endl;
	std::cout << "Time taken for this step was: " << (double) tm_misc / CLOCKS_PER_SEC << " seconds" << std::endl;

	// FILE_1

	data.open ( ("data/"+file_1).c_str() );	// open file_1
	if ( data.is_open() )	// if file_1 is open, carry on
	{
		// initialize a string for each of the three components on each line of the input file being read
		std::string x;
		std::string y;
		std::string value;

		std::cout << ("Data being read in from "+file_1).c_str() << "..." << std::endl;
		while ( std::getline(data,sLine) )	// while line being read is not end of the file
		{
			std::stringstream data(sLine);	// hold line in data
			data >> x >> y >> value;		// split line into three components

			if ( !sLine.empty() )	// if line is not empty, carry on
			{
				int column = stoi(x);			// convert from string to int
				int row = stoi(y);				// convert from string to int
				double potential = stod(value);	// convert from string to double

				grid_a[column][row] = potential;	// record value at column x row y in file_1 into grid_a[x][y]
			//	std::cout << "X: " << column << "Y: " << row << " grid[x][y]" << grid[column][row] << std::endl;	// uncomment to see progress
			}
			else	// if line is empty, go back to start of while loop to skip over blank lines
			{
				continue;
			//	std::cout << "empty string" << std::endl;	// uncomment to see progress
			}		
			// std::cout << "Working..." << std::endl;		// uncomment to see progress
		}
		data.close();	// once reading is done, close the file stream
		std::cout << "Data reading finished." << std::endl;

		tm_read1 = clock() - tm_misc;	// record time taken for reading file_1

		std::cout << "Time taken for reading file 1: " << (double) tm_read1 / CLOCKS_PER_SEC << " seconds" << std::endl;
	}
	else	// if file cannot be opened, exit the program after letting the user know what happened
	{
		std::cout << ("Unable to open "+file_1+", exiting now.").c_str() << std::endl;
		exit(EXIT_FAILURE);
	}

	// FILE_2

	data.open ( ("data/"+file_2).c_str() );	// open file_2
	if ( data.is_open() )	// if file_2 is open, carry on
	{
		// initialize a string for each of the three components on each line of the input file being read
		std::string x;
		std::string y;
		std::string value;

		std::cout << ("Data being read in from "+file_2).c_str() << "..." << std::endl;
		while ( std::getline(data,sLine) )	// while line being read is not the end of the file
		{
			std::stringstream data(sLine);	// hold line in data
			data >> x >> y >> value;	// split line into three components

			if ( !sLine.empty() )	// if line is not empty, carry on
			{
				int column = stoi(x);			// convert from string to int
				int row = stoi(y);				// convert from string to int
				double potential = stod(value);	// convert from string to double

				grid_b[column][row] = potential;	// record value at column x row y in file_2 into grid_b[x][y]
			//	std::cout << "X: " << column << "Y: " << row << " grid[x][y]" << grid[column][row] << std::endl;	// uncomment to see progress
			}
			else
			{
				continue;
			//	std::cout << "empty string" << std::endl;	// uncomment to see progress
			}		
			// std::cout << "Working..." << std::endl;		// uncomment to see progress
		}
		data.close();	// once reading is done, close the file stream
		std::cout << "Data reading finished." << std::endl;

		tm_read2 = clock() - tm_read1;	// record time taken for reading file_2

		std::cout << "Time taken for reading file 2: " << (double) tm_read2 / CLOCKS_PER_SEC << " seconds" << std::endl;
	}
	else	// if file cannot be opened, exit the program after letting the user know what happened
	{
		std::cout << ("Unable to open "+file_2+", exiting now.").c_str() << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "Calculating difference..." << std::endl;

	double epsilon = 0.000000001;	// define variable epsilon as the minimum value at a point before the point is treated as being zero

	// Calculate difference at each point between the two input files
	// If difference is miniscule (i.e. less than epsilon), 
	// set it to be zero to avoid possible issues with floating point errors
	for (int x = 0 ; x < columns ; x++)
		{ //loop over x co-ordinates
	//		if ( x % 20 == 0)
	//		{
	//			std::cout << "x = " << x << std::endl;
	//		}
			for (int y = 0; y < rows ; y++)
			{ //loop over y co-ordinates
				if ( grid_a[x][y] <= epsilon && grid_b[x][y] <= epsilon ) // if both points have values less than epsilon, set the difference to zero to avoid dividing by tiny values (could cause issues with floating point arithmetic)
				{
					grid_diff[x][y] = 0;
				}
				else // if both points greater than epsilon, set grid_diff[x][y] to be the difference between the two points as a percentage of the average of the two points
				{
					grid_diff[x][y] = std::abs(2 * (grid_a[x][y] - grid_b[x][y]) / (grid_a[x][y]+grid_b[x][y]))*100;	
				}
			}
		}
	std::cout << "Difference calculated." << std::endl;

	tm_calculate_diff = clock() - tm_read2;	// record time taken to calculate the difference between grid_a and grid_b at each point

	std::cout << "Time taken to calculate difference: " << (double) tm_calculate_diff / CLOCKS_PER_SEC << " seconds" << std::endl;

	// Write the contents of grid_diff into output file called difference.dat in data directory
	// The output file is in the same format as the input files, i.e. three columns of data
	// The first column is the x coordinate, the second column is the y coordinate
	// and the third column is the difference between the two points in the input files at those coordinates
		
	std::cout << "Writing results to data/difference.dat..." << std::endl;

	std::ofstream output;	// create output file stream called output (very creative, I know)
	output.open("data/difference.dat");	// open data/difference.dat in the output stream

	for(int x = 0 ; x < columns ; x++)	// Loop over x coordinates (columns)
	{
		for(int y = 0 ; y < rows ; y++)	// Loop over y coordinates (rows)
		{
			output << x << "\t" << y << "\t" << grid_diff[x][y] << std::endl;	// output the x and y coordinates followed by the value of grid_diff[x][y]
		}	
		output << std::endl;	// make an empty line before new x values start being written (this is so gnuplot knows what is going on)
	}

	output.close();	// close the output file stream

	std::cout << "Results written." << std::endl;

	tm_write = clock() - tm_calculate_diff;	// record the time taken for writing to the output file
	tm_total = clock() - tm_initial;		// record the total time taken

	std::cout << "Time taken to write results to file: " << (double) tm_write / CLOCKS_PER_SEC << " seconds" << std::endl;
	std::cout << "Total time elapsed while executing the program: " << (double) tm_total / CLOCKS_PER_SEC << " seconds" << std::endl;

	return 0;	// program finished
}
