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
    rocksdb::Status s1 = db_rocks->Get(rocksdb::ReadOptions(), info.INFO_PREFIX_NEXT_BLOCKID, &lastBlockId);
    std::string lastTxId;
    rocksdb::Status s2 = db_rocks->Get(rocksdb::ReadOptions(), info.INFO_PREFIX_NEXT_TXID, &lastTxId);
    std::string lastAddressId;
    rocksdb::Status s3 = db_rocks->Get(rocksdb::ReadOptions(), info.INFO_PREFIX_NEXT_ADDRESSID, &lastAddressId);

    if (s1.ok() && s2.ok() && s3.ok()) {
        printf("Rocks last %s\n Rocks last tx %s\n", lastBlockId.c_str(), lastTxId.c_str());
        info.nextBlockId = (size_t)std::stoi( lastBlockId );
        info.nextTxId = (size_t)std::stoi( lastTxId );
        info.nextAddressId = (size_t)std::stoi( lastAddressId );

    }else {
        printf("keys not found \n");
        info.nextBlockId = 1;
        info.nextTxId = 1;
        info.nextAddressId = 1;
    }

    // if the value are set to initial values, we need to start from the beginning.
    if (info.nextBlockId == 1 && info.nextTxId == 1) {
        // create database tables
        createRDBMSSchema(db_sqlite);
    }



    Parser p("/home/prabushitha/.ethereum/rinkeby/geth/chaindata");


    std::cout << "starting block id \t" << info.nextBlockId << std::endl;
    std::cout << "starting tx id    \t" << info.nextTxId << std::endl;
    std::cout << "starting addr id  \t" << info.nextAddressId << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "-----------PARSING STARTED-----------" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    // lets store blocks!!!
    size_t max_blocks = info.nextBlockId+100000;
    for(size_t i=info.nextBlockId; i<max_blocks; i++) {
        Block b = p.getBlock(i);
        storeBlockInRDBMS(db_sqlite, db_rocks, info, p, b); // db_rocks
    }
    std::cout << "----------PARSING COMPLETED----------" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "next block id \t" << info.nextBlockId << std::endl;
    std::cout << "next tx id    \t" << info.nextTxId << std::endl;
    std::cout << "next addr id  \t" << info.nextAddressId << std::endl;

    // save last ids
    s1 = db_rocks->Put(rocksdb::WriteOptions(), info.INFO_PREFIX_NEXT_BLOCKID, std::to_string(info.nextBlockId));
    s2 = db_rocks->Put(rocksdb::WriteOptions(), info.INFO_PREFIX_NEXT_TXID, std::to_string(info.nextTxId));
    // close databases
    sqlite3_close(db_sqlite);
    delete db_rocks;

    return 0;
}