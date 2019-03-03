#!/bin/sh 

[ -d debug ] || mkdir debug
set -e
cd debug
if [ $# -eq 0 ]; then 
	cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
	make
else
	cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug --target $1 ..
	make $1
fi
