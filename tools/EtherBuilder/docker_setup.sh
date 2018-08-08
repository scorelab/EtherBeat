#!/usr/bin/env bash

# dependencies
#gtest
cd /usr/src/gtest/
sudo cmake -DBUILD_SHARED_LIBS=ON
sudo make
sudo cp *.so /usr/lib

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

export LD_LIBRARY_PATH=/usr/local/lib

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

export LD_LIBRARY_PATH=/usr/lib

# project
git clone https://github.com/prabushitha/EtherBeat.git
cd EtherBeat
git checkout parser
cd tools/EtherExtractor
chmod +x run.sh
# ./run.sh
