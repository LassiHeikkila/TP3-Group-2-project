CC = g++
CFLAGS = -g -Wall
LIBS = -lm

DEPS = headers.h bitmap_image.hpp
OBJ = bitmap.o dataout.o fdm.o gnuplot.o main.o 

default: estatics

estatics: $(OBJ)
	@echo "Linking object files..."
	@$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	@echo "Done!"

%.o: %.cpp $(DEPS)
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)
