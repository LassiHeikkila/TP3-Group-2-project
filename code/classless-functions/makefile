CC = g++
CFLAGS = -g -Wall
LIBS = -lm

DEPS = headers.h bitmap_image.hpp
OBJ = bitmap.o data_out.o fdm.o plot.o memory.o main.o

DIRECTORIES = data plots

default: estatics

estatics: $(OBJ)
	@echo "Linking object files..."
	@$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	@echo "Done!"

%.o: %.cpp $(DEPS) | $(DIRECTORIES)
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)
	
$(DIRECTORIES):	
	@mkdir plots
	@mkdir data

.PHONY: clean

clean:
	@echo "Removing object files!"	
	@rm -f *.o
	@echo "Done!"
