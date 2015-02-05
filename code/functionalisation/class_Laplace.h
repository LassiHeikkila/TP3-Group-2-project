// Class definition/declaration/initialization/whatever this is

class Laplace
{
public:
  std::vector< std::vector <float> > array;
  
  // Member function declarations
  void SetArraySize(int dim_x, int dim_y);
  // void InitializePotential(float potential_black,float potential_red, float potential_green, float potential_blue);
  bool Circle(int i, int j, float r, float dx, float dy, float d, float h);
  void ViewArray();
  void OutputField(float dx, float dy);
  void OutputPotential(float dx, float dy);
  void OutputGraph(float d, float h);  
};

// Member function definitions

void Laplace::SetArraySize(int dim_x, int dim_y) // Works
// creates an array (made up of vectors of vectors) of size dim_x-by-dim_y
{
  for ( int i = 0 ; i < dim_x ; i++ )
    {
      array.push_back(std::vector<float>()); // Create dim_x empty rows to start with
      // std::cout << "1" << std::endl; // uncomment to see progress for debug purposes
    }  
  for ( int i = 0 ; i < dim_x ; i++ )
    {
      // std::cout << "2" << std::endl; // uncomment to see progress for debug purposes
      for ( int j = 0 ; j < dim_y ; j++ )
	{
	  // std::cout << "3" << std::endl; // uncomment to see progress for debugging
	  array[i].push_back(j); // create a new vector for each vector
	  array[i][j] = 0; // initialize to 0
	}
    }
}

/* void Laplace::InitializePotential(float potential_black, float potential_red, float potential_green, float potential_blue) // Works
// initializes array to a linearly decreasing potential from one plate at +V to the other at -V
{
  
}
*/
bool Laplace::Circle(int i, int j, float r, float dx, float dy, float d, float h) // Works
// returns true if point (i,j) is inside circle of radius r
// returns false otherwise
{
  if ( (pow(d/2 - i*dx,2) + pow(h/2 - j*dy,2)) <= pow(r,2))
    {
      return true;
    }
  else
    {
      return false;
    } 
}

void Laplace::ViewArray() // Works
// outputs array to terminal 
{
  std::cout << "This is what the array looks like: " << std::endl;
  for ( unsigned int i = 0 ; i < array.size() ; i++ )
    {
      for ( unsigned int j = 0 ; j < array[i].size() ; j++ )
	{
	  std::cout << "\t" << array[i][j];
	}
      std::cout << "\t" << std::endl;
    }
}

void Laplace::OutputField(float dx, float dy) // Works 
// outputs array to a file called "output.dat"
{
  std::ofstream myfile;
  myfile.open("field.dat");
  // std::cout << "File is open" << std::endl;
  if (myfile.is_open())
    {
      //	myfile << "test1"; // uncomment for debugging
      for ( unsigned int i = 0 ; i < array.size()-1 ; i++ )
	{
	  for ( unsigned int j = 0 ; j < array[i].size()-1 ; j++ )
	    {
	      myfile << i*dx << "\t" << j*dy << "\t" << -(array[i+1][j]-array[i][j])/dx << "\t" << -(array[i][j+1]-array[i][j])/dy << std::endl;
	    }
	  myfile << std::endl;
	}
      //	myfile << "test2"; // uncomment for debugging
      myfile.close();
      // std::cout << "File is closed" << std::endl;
    }
  else 
    {
      std::cout << "I couldn't open the file 'field.dat', so I will exit the program now." << std::endl;
      exit(EXIT_FAILURE);
    }
}

void Laplace::OutputPotential(float dx, float dy) // Works
{
  std::ofstream myfile;
  myfile.open("potential.dat");
  if (myfile.is_open())
    {
      for ( unsigned int i = 0 ; i < array.size() ; i++ )
	{
	  for ( unsigned int j = 0 ; j < array[i].size() ; j++ )
	    {
	      myfile << i*dx << "\t" << j*dy << "\t" << array[i][j] << std::endl;
	    }
	  myfile << std::endl;
	}
      myfile.close();
    }
  else
    {
      std::cout << "I couldn't open potential.dat, so I will exit the program now." << std::endl;
      exit(EXIT_FAILURE);
    }
}

void Laplace::OutputGraph(float d, float h) // Works
{
// Remember to add \n after each line

  float ratio = h/d;

  FILE* gnuplotpipe = popen ("gnuplot -persistent", "w"); // popen a file called gnuplotpipe in write mode to run gnuplot
    
  fprintf(gnuplotpipe, "set terminal postscript landscape enhanced color\n"); // set terminal to whatever
  fprintf(gnuplotpipe, "set output 'field.eps'\n"); // set output to field.eps
  fprintf(gnuplotpipe, "set xrange[-1/%f:%f]\nset yrange[-1/%f:%f]\n",d,d,h,h); // set xrange and yrange
  fprintf(gnuplotpipe, "set size ratio %f\n",ratio); // set plot shape to rectangle with side ratio = d:h
  fprintf(gnuplotpipe, "plot 'field.dat' with vectors head filled lt 3\n"); // plot field.dat with vectors
  fprintf(gnuplotpipe, "set output 'potential.eps'\n"); // set output to potential.eps
  fprintf(gnuplotpipe, "set pm3d map\n"); // set output to heat-map type thing
  fprintf(gnuplotpipe, "splot 'potential.dat'\n"); // plot potential.dat
  
  pclose(gnuplotpipe); // pclose the file 
}
