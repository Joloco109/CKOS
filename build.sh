#!/bin/sh 

mkdir build
set -e
cd build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
make
