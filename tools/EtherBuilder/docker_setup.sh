#!/usr/bin/env bash

# dependencies
# leveldb
# sudo apt-get install libsnappy-dev

wget https://leveldb.googlecode.com/files/leveldb-1.9.0.tar.gz
tar -xzf leveldb-1.9.0.tar.gz
cd leveldb-1.9.0
make
sudo mv libleveldb.* /usr/local/lib
cd include
sudo cp -R leveldb /usr/local/include
sudo ldconfig

#secp256k1
# sudo apt-get install autoconf autogen libtool
git clone https://github.com/bitcoin-core/secp256k1.git
cd secp256k1
./autogen.sh
./configure
make
./tests
sudo make install  # optional

# gtest
# sudo apt-get install libgtest-dev

# project
git clone https://github.com/prabushitha/EtherBeat.git
cd EtherBeat
git checkout parser
cd tools/EtherExtractor
chmod +x run.sh
./run.sh