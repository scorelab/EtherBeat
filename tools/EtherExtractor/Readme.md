# EtherExtractor

A tool which connects to the leveldb ethereum blockchain (currently supports Geth synced chain) and extract block related information. This doesn't need any RPC or IPC endpoints to be active, it directly acccess leveldb to get information. EtherExtractor is an independent tool, which can be extended to applications which needs to access blockchain.


## Quick Guide
#### Dependencies
Install the below dependencies in your environment.
 - leveldb
 - cryptopp
 - secp256k1
 - gtest

#### Running the project
- Compile as a <b>Library</b> or use as a <b>Standalone Application</b>


## How to use EtherExtractor?

There are 2 ways of using EtherExtractor.
#### 1. As a Library
You can use this as a library.

##### Compiling as a library
```sh
#!/usr/bin/env bash

rm -rf build
mkdir build
cd build
cmake .. -DIS_LIBRARY=TRUE
make
```
After compile using above commands, you'll find the library .so file in `EtherExtractor/build/libethereum_extractor.so`


(Check EtherBuilder which uses EtherExtractor as a library for a sample implementation)

#### 2. Standalone
You can directly use the source files and write your own application. (See `src/main.cpp`). See `CMakeLists.txt` if you want to change the make instructions.

##### Compiling and running

```sh
#!/usr/bin/env bash

export LD_LIBRARY_PATH=/usr/local/lib

# Compiling

rm -rf build
mkdir build
cd build
cmake ..
make

# Running
./ethereum_extractor
```

##### Example usage :
```cpp
#include "extractor.h"
...
// initialize ether extractor
EtherExtractor extractor("/home/user/.ethereum/geth/chaindata");

// get a block by number
Block block1 = extractor.getBlock(189154);

// get a block by hash
Block block2 = extractor.getBlock("0xe01eb24a18d0c117409a7b8946ed16b4c485ed79746644b89250384ed9bf2fda");

// get a transaction receipt by hash
TransactionReceipt tr = extractor.getTransactionReceipt("0xfddd30b69b0c4f918785e83c8b8ddd21fe3e2019a142fc382b88fa5de6c88fa2");

// loop over transactions
for (Transaction transaction : block1.transactions) {
	// get the value transferred
	double value = transaction.getValue();
}


```
Check `EtherExtractor/include/block.h` for available methods of block, block header, transaction & transaction receipt.

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

<hr/>

#### Useful Links to understand the development
 - [Ethereum Blockchain](https://www.hashhack.it/posts/source-r-ethereum)
 - [Geth database (leveldb) access Go-lang](https://github.com/ethereum/go-ethereum/blob/be1c56e244abc2532b4caad05f7f655012114cb3/core/database_util.go)
 - [Signing a transaction](https://github.com/ethereum/go-ethereum/blob/master/core/types/transaction_signing.go#L72)
 - Recover public key from signature
   - [libdevcrypto](https://github.com/ethereum/cpp-ethereum/blob/develop/libdevcrypto/Common.cpp)
   - [crypto/signature](https://github.com/ethereum/go-ethereum/blob/master/crypto/signature_nocgo.go)
   - [crypto/secp256k1](https://github.com/ethereum/go-ethereum/blob/master/crypto/secp256k1/secp256.go)
   - [ethereumjs/ethereumjs-tx](https://github.com/ethereumjs/ethereumjs-tx/blob/master/index.js)
   - [determine-the-from-address](https://ethereum.stackexchange.com/questions/36051/given-only-a-raw-transaction-how-can-i-determine-the-from-address-that-signed-i)
