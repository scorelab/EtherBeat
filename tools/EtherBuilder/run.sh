#!/usr/bin/env bash

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

./EtherBuilder $1 $2
