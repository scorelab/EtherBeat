#include "parser.h"
#include "block_store.h"
#include <sqlite3.h>
#include <iostream>
#include <stdio.h>


int main() {
    // Connect to SQLite
    sqlite3 *db_sqlite;
    int rc = sqlite3_open("/tmp/dbsqlite/ethereum_blockchain.db", &db_sqlite);

    if( rc ) {
        fprintf(stderr, "SQLite db failed : %s\n", sqlite3_errmsg(db_sqlite));
        return(0);
    } else {
        fprintf(stderr, "SQLite db connected\n");
    }

    // Connection to Rocksdb
    rocksdb::DB* db_rocks;
    rocksdb::Options options;
    options.create_if_missing = true;
    rocksdb::Status status = rocksdb::DB::Open(options, "/tmp/dbrocks", &db_rocks);
    if(status.ok()){
        printf("Rocksdb connected\n");
    }else{
        printf("Rocksdb failed : %s \n",  status.ToString().c_str());
    }

    struct BuilderInfo info;


    // get last info
    std::string lastBlockId;
    rocksdb::Status s1 = db_rocks->Get(rocksdb::ReadOptions(), info.INFO_PREFIX_LAST_BLOCKID, &lastBlockId);
    std::string lastTxId;
    rocksdb::Status s2 = db_rocks->Get(rocksdb::ReadOptions(), info.INFO_PREFIX_LAST_TXID, &lastTxId);

    if (s1.ok() && s2.ok()) {
        printf("Rocks last %s\n Rocks last tx %s\n", lastBlockId.c_str(), lastTxId.c_str());
        info.lastBlockId = (size_t)std::stoi( lastBlockId );
        info.lastTxId = (size_t)std::stoi( lastTxId );

    }else {
        printf("keys not found \n");
        info.lastBlockId = 1;
        info.lastTxId = 1;
    }

    // if the value are set to initial values, we need to start from the beginning.
    if (info.lastBlockId == 1 && info.lastTxId == 1) {
        // create database tables
        createRDBMSSchema(db_sqlite);
    }



    Parser p("/home/prabushitha/.ethereum/rinkeby/geth/chaindata");

    printf("Initial id: %d \nInitial Tx id: %d \n", info.lastBlockId, info.lastTxId);
    // lets store blocks!!!
    size_t max_blocks = info.lastBlockId+1000;
    for(size_t i=info.lastBlockId; i<max_blocks; i++) {
        Block b = p.getBlock(i);
        storeBlockInRDBMS(db_sqlite, db_rocks, info, p, b); // db_rocks
    }
    printf("Next block id: %d \nNext Tx id: %d \n", info.lastBlockId, info.lastTxId);

    // save last ids
    s1 = db_rocks->Put(rocksdb::WriteOptions(), info.INFO_PREFIX_LAST_BLOCKID, std::to_string(info.lastBlockId));
    s2 = db_rocks->Put(rocksdb::WriteOptions(), info.INFO_PREFIX_LAST_TXID, std::to_string(info.lastTxId));
    // close databases
    sqlite3_close(db_sqlite);
    delete db_rocks;

    return 0;
}