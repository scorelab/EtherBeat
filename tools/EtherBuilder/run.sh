#!/usr/bin/env bash
#project_dir='./tools/parser'
#cd ${project_dir}

export LD_LIBRARY_PATH=/usr/local/lib

rm -rf build
mkdir build
cd build
cmake ..
make

printf "Build Finish\n-----------------------------------------------\nProgram Started \n-----------------------------------------------\n\n"

rm -R ../data/dbrocks
rm -R ../data/dbsqlite

mkdir ../data/dbrocks
mkdir ../data/dbsqlite

./EtherBuilder
