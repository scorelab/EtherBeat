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

rm -R /tmp/dbrocks
rm -R /tmp/dbsqlite

mkdir /tmp/dbrocks
mkdir /tmp/dbsqlite

./EtherBuilder 100000
