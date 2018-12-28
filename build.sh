#!/bin/sh 

mkdir build
set -e
cd build
if [ $1 -eq 0 ]
then 
	cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
	make
else
	cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug --target $1 ..
	make $1
fi
