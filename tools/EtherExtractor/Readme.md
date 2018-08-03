# EtherExtractor

A tool which connects to the leveldb ethereum blockchain (currently supports Geth synced chain) and extract block related information. This doesn't need any RPC or IPC endpoints to be active, it directly acccess leveldb to get information. EtherExtractor is an independent tool, which can be extended to applications which needs to access blockchain.


## Quick Guide
### Dependencies
Install the below dependencies in your environment.
 - leveldb
   - In Arch use, `pacman -S leveldb`
 - cryptopp
   - In Arch use, `pacman -S crypto++`
 - secp256k1
   - In Arch use, `pacman -S libsecp256k1`
 - gtest
   - In Arch use, `pacman -S gtest`

### Running the project
 - `./run.sh`

### Create a library and using it in your project
 - `./build_library.sh`

<hr />

## How to use EtherExtractor?

There are 2 ways of using EtherExtractor. 
#### 1. Standalone  
You can directly use the source files and write your own application. (See `src/main.cpp`). See `CMakeLists.txt` and `run.sh` . Below is an example usage.

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

#### 2. As a Library 
You can use this as a library.  Compile using `build_library.sh`. Then you'll find the library .so file in `EtherExtractor/build/libethereum_extractor.solibethereum_extractor.so`

(Check EtherBuilder which uses EtherExtractor as a library for a sample implementation) 


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
