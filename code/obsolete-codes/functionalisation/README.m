The image that is used for determining the boundary conditions can be called anything.
When running the program, it currently requires 11 command line arguments, the first 5 of which can be anything (disabled currently).
The sixth is the number of iterations to run.
The seventh is the potential at black pixels.
The eight is the potential at red pixels.
The nineth is the potential at green pixels.
The tenth is the potential at blue pixels.
The eleventh is the name of the image.

So e.g. you would call the program after compiling it as "test" as follows:
  ./test 0 0 0 0 0 1000 2 -2 0 0 image.bmp
  
  where 1000 is the number of iterations to run, 2 is the potential at black pixels, -2 is the potential at red pixels, 0 at green and blue pixels, and the image is called image.bmp.
  
