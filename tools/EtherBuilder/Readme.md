# Etherbeat Builder

Tool to store ethereum blockchain in order to support analysis. SQLite database and Rocksdb has been used. 

## Dependencies
- SQLite3
- Rocksdb
- pthread
- ethereum_extractor (tools/EtherExtractor)


*Note - All the dependencies for EtherExtractor should be available

## Quick Guide

### EtherExtractor Setup

1. Goto `../tool/EtherExtractor`
2. Compile EtherExtractor as a library.

### SQLite Setup

1. Install SQLite3 in your environment

2. Create directory for sqlite database
   - `mkdir /tmp/dbsqlite`

### Rocksdb Setup

1. Download the latest rocksdb source code from github
   - `git clone https://github.com/facebook/rocksdb.git`

2. Go into rocksdb folder
   - `cd rocksdb`

3. build
   - `CXXFLAGS="-Wno-error" DEBUG_LEVEL=0 make shared_lib install-shared`

4. set to `LD_LIBRARY_PATH` (usually rocksdb installed in /usr/lib but some environments like arch it's installed in /usr/local/lib)
   - `export LD_LIBRARY_PATH=/usr/lib`

5. create a directory for rocksdb database
   - `mkdir /tmp/dbrocks`

### Build EtherBuilder

Use below commands to build
```sh
export LD_LIBRARY_PATH=/usr/local/lib

rm -rf build
mkdir build
cd build

cmake ..
make
```

### Running EtherBuilder

Goto the build directory and execute EtherBuilder. Execution command format,

`./EtherBuilder numblocks /pathto/chaindata /pathto/dbsqlite /pathto/dbrocks`


For an example, Let's parse 100000 blocks. you can change the value. Also give the correct path to the blockchain containing chaindata directory.

`./EtherBuilder 1000 /home/user/.ethereum/geth/chaindata /tmp/dbsqlite /tmp/dbrocks`


## Docker Guide

1. docker build
   - `docker build -t ether_builder .`
2. docker run (note that `/home/user/.ethereum` is the host directory containing synced blockchain)
   - `docker run -v /home/user/.ethereum:/mnt -it ether_builder /bin/bash`
3. compile other dependencies
   - `cd /usr/src`
   - `./docker_setup.sh`
4. export library path
   - `export LD_LIBRARY_PATH=/usr/local/lib`
5. run (Let's parse 100000 blocks. you can change the value)
   - `cd /usr/src/EtherBeat/tools/EtherBuilder/build`
   - `./EtherBuilder 100000 /mnt/rinkeby/geth/chaindata /mnt/dbsqlite /mnt/dbrocks`

## Developer Guide

#### Running tests
```sh
#!/usr/bin/env bash
export LD_LIBRARY_PATH=/usr/local/lib
# Compiling
rm -rf build
mkdir build
cd build
cmake ..
make

# Running tests
./test_extractor
```

#### Coding standards
Follow [Google's C++ style guide](http://google.github.io/styleguide/cppguide.html). To enforce standards, you can use [cpplint](https://github.com/cpplint/cpplint)



## Functionality list

1. Store Blocks, Transactions, Transaction Receipts in 3 sqlite tables
2. Store Transaction id's related to a block in sqlite table
3. Store Accounts in Sqlite
4. Block hash to block id (same as number) mapping in rocksdb
5. Transaction hash to transaction id mapping in rocksdb
6. Store Account to Accountid mapping in rocksdb
7. Store Transaction From-To addresses like a graph in sqlite or rocksdb
