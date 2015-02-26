#!/bin/bash

for i in {10..20}
	do
		SOR=$(( $i / 10 ))
		echo "Solving System A with relaxation parameter $SOR..."
		./estatics systems/SystemA.bmp 0 10 -10 0 100000 $SOR 1e-9
		mv data/potential.dat data/relaxation/potential_$SOR.dat
		mv data/field.dat data/relaxation/field_$SOR.dat
		mv data/runtime.dat data/relaxation/runtime_$SOR.dat
done
