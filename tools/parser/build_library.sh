#!/usr/bin/env bash
#project_dir='./tools/parser'
#cd ${project_dir}

rm -rf build
mkdir build
cd build
cmake .. -DIS_LIBRARY=TRUE
make
printf "Library Built Finish\n\n"
