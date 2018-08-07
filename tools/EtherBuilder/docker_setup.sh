#!/usr/bin/env bash

# dependencies

#secp256k1
# sudo apt-get install autoconf autogen libtool
git clone https://github.com/bitcoin-core/secp256k1.git
cd secp256k1
./autogen.sh
./configure
make
./tests
sudo make install  # optional
export LD_LIBRARY_PATH=/usr/local/lib
# gtest
# sudo apt-get install libgtest-dev
cd ..
# project
git clone https://github.com/prabushitha/EtherBeat.git
cd EtherBeat
git checkout parser
cd tools/EtherExtractor
# chmod +x run.sh
# ./run.sh
