#!/usr/bin/env bash

# dependencies

# cmake
wget http://www.cmake.org/files/v3.10/cmake-3.10.1.tar.gz 
tar -xvzf cmake-3.10.1.tar.gz 
cd cmake-3.10.1/ 
./configure 
make
sudo make install
cd ../

# cryptopp
git clone https://github.com/weidai11/cryptopp.git
cd cryptopp
make
make test
sudo make install
cd ../

#secp256k1
git clone https://github.com/bitcoin-core/secp256k1.git
cd secp256k1
./autogen.sh
./configure --enable-module-recovery
make
./tests
sudo make install  # optional
cd ../

# sqlite
wget http://www.sqlite.org/sqlite-autoconf-3071502.tar.gz

tar xvfz sqlite-autoconf-3071502.tar.gz

cd sqlite-autoconf-3071502

./configure --prefix=/usr/local

make

make install
cd ../

# rocksdb
git clone https://github.com/facebook/rocksdb.git

cd rocksdb

DEBUG_LEVEL=0 make shared_lib install-shared

cd ../

#gtest
cd gtest/
sudo cmake -DBUILD_SHARED_LIBS=ON
sudo make
sudo cp *.so /usr/lib
cd ../

# project setup
git clone https://github.com/prabushitha/EtherBeat.git
cd EtherBeat
git checkout parser
cd tools/EtherExtractor

# build EtherExtractor as a library
rm -rf build
mkdir build
cd build
cmake .. -DIS_LIBRARY=TRUE
make
printf "EtherExtractor Library Built Finished\n\n"

# build EtherBuilder
cd /usr/src/EtherBeat/tools/EtherBuilder
mkdir build
cd build
cmake ..
make

# create directories for databases
rm -R /mnt/dbrocks
rm -R /mnt/dbsqlite

mkdir /mnt/dbrocks
mkdir /mnt/dbsqlite


