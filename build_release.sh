#!/bin/sh 

rm -r release
mkdir release
set -e
cd release
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
make
cp compile_commands.json ..
