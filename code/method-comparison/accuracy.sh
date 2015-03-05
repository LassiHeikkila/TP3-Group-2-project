#!/bin/bash

# bash script runs each of the five methods
# for different number of iterations
# specified as command line arguments to cpp file

# each cpp file writes out data files for:
# - CPU time vs its,
# - convergence vs its,
# - error vs its,
# - and potential at last iteration,
# in the data directory.

#for each file
for file in jacobi gauss checker sor checker_sor
do

#compile the file
g++ -o $file.exe $file.cpp

 #for specified sequence of numbers
 for i in `seq 1 50`
 do

  #remove data file before each loop
  if [ -e data/$file.dat ] ; then
	rm data/$file.dat
  fi

  #run executable for specific number of iterations
  echo "Running" $file.exe $((200*i)) "times..."
  echo ""
   ./$file.exe $((200*i))
  echo ""

 done
done

echo "Finished!"

exit
