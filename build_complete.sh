#!/bin/sh 

rm -r debug
mkdir debug
set -e
cd debug
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
make
cp compile_commands.json ..
