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
./configure
make
./tests
sudo make install  # optional
cd ../

export LD_LIBRARY_PATH=/usr/local/lib

# project
git clone https://github.com/prabushitha/EtherBeat.git
cd EtherBeat
git checkout parser
cd tools/EtherExtractor
chmod +x run.sh
# ./run.sh
