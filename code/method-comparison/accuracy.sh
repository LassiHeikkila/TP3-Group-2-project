#!/bin/bash

# bash script runs each method for
# command line arguments:
# radius, max_its, convergence
# for decreasing convergences

for file in jacobi.exe gauss.exe sor.exe checker.exe
do
 for i in 1e-2 1.5e-3 1e-3 1.5e-4 1e-4 1.5e-5 1e-5
 do
  echo "Running" $file "for convergence" $i"..."
  echo ""
  ./$file $1 $2 $i
  echo ""
 done
done

echo "Finished!"

exit
