CC = g++
CFLAGS = -g -Wall
LIBS = -lm

DEPS = headers.h bitmap_image.hpp
OBJ = bitmap.o dataout.o fdm.o gnuplot.o main.o 

default: estatics

estatics: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

%.o: %.cpp $(DEPS)
	$(CC) -c $@ $< $(CFLAGS) $(LIBS)