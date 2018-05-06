#!/usr/bin/env bash
rm -rf build
mkdir build
cd build
cmake ..
make
printf "Build Finish\n-----------------------------------------------\nProgram Started \n-----------------------------------------------\n\n"
./ethereum_parser
