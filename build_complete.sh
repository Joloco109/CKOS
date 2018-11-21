#!/bin/sh 

rm -r build
mkdir build
set -e
cd build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
make
cp compile_commands.json ..
