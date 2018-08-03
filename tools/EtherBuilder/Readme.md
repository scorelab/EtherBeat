# Etherbeat Builder

Tool to store ethereum blockchain in order to support analysis. SQLite database and Rocksdb has been used. 

## Dependencies
- SQLite3
- Rocksdb
- pthread
- ethereum_extractor (tools/EtherExtractor)

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

5. create a directory for rocksdb
   - `mkdir /tmp/dbrocks`


### Running Builder

`./run.sh`


## Functionality list

1. Store Blocks, Transactions, Transaction Receipts in 3 sqlite tables
2. Store Transaction id's related to a block in sqlite table
3. Store Accounts in Sqlite
4. Block hash to block id (same as number) mapping in rocksdb
5. Transaction hash to transaction id mapping in rocksdb
6. Store Account to Accountid mapping in rocksdb
7. Store Transaction From-To addresses like a graph in sqlite or rocksdb