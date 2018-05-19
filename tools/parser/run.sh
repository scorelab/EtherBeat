#!/usr/bin/env bash
project_dir='./tools/parser'
cd ${project_dir}

rm -rf build
mkdir build
cd build
cmake ..
make
printf "Build Finish\n-----------------------------------------------\nProgram Started \n-----------------------------------------------\n\n"
./ethereum_parser
