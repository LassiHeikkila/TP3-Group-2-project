#!/bin/bash

# bash script runs each method for
# command line arguments:
# radius, max_its, convergence

for file in jacobi.exe gauss.exe sor.exe checker.exe
do
./$file $1 $2 $3
done

exit
