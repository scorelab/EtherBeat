/*
 * Created by prabushitha on 6/12/18.
 * Copyright [2018] <Umesh Jayasinghe>
*/
#ifndef TOOLS_ETHERBUILDER_INCLUDE_BLOCK_STORE_H_
#define TOOLS_ETHERBUILDER_INCLUDE_BLOCK_STORE_H_

#include "rocksdb/db.h"
#include "extractor.h"
#include <sqlite3.h>
#include <pthread.h>
#include <iostream>
#include <cstdarg>
#include <cstdio>
#include <sstream>
#include <string>


#define TX_BUFFER_SIZE 1000

struct BuilderInfo {
    std::string KEY_NEXT_BLOCKID = "info_next_block_id";
    std::string KEY_NEXT_TXID = "info_next_tx_id";
    std::string KEY_NEXT_ADDRESSID = "info_next_address_id";

    size_t nextBlockId;
    size_t nextTxId;
    size_t nextAddressId;

    std::string PREFIX_BLOCK = "block_";
    std::string PREFIX_TX = "tx_";
    std::string PREFIX_ADDRESS = "address_";
};

struct TransactionBuffer {
    Transaction buffer[TX_BUFFER_SIZE];
    int next_in = 0;
    int next_out = 0;
    int occupied = 0;
    pthread_mutex_t mutex {};
    pthread_cond_t wait_on_no_tx {};
    pthread_cond_t wait_on_full_tx {};
};

struct StoreBasicArgs {
    sqlite3_stmt * stmt_block;
    sqlite3_stmt * stmt_tx;
    sqlite3_stmt * stmt_blocktx;
    sqlite3_stmt * stmt_txreceipt;
    sqlite3_stmt * stmt_fromto;
    rocksdb::DB* db_rocks;
    struct BuilderInfo &info;
    EtherExtractor &extractor;
    bool * isMasterOver;
    struct TransactionBuffer * txbuffer;
};

int run_sql_query(sqlite3 *db, std::string sql, std::string title);

void createRDBMSSchema(sqlite3 *db);
void bindToBlockSql(sqlite3_stmt * stmt, Block b);

void bindToTxSql(sqlite3_stmt * stmt,
                Transaction transaction,
                 size_t blockId,
                 size_t txId);

void bindToBlockTxSql(sqlite3_stmt * stmt,
                      size_t blockId,
                      size_t txId);

void bindToTxReceiptSql(sqlite3_stmt * stmt,
                        TransactionReceipt receipt,
                        size_t txId);

void bindToFromtoSql(sqlite3_stmt * stmt,
                     size_t from,
                     size_t to,
                     double amount,
                     size_t txid);

size_t getHashId(rocksdb::DB* db_rocks,
                 std::string hash);

bool updateHashId(rocksdb::DB* db_rocks,
                  std::string hash, int id);

bool isAddressValid(std::string address);

size_t updateAndGetAccountHashId(rocksdb::DB* db_rocks,
                                 std::string hash,
                                 struct BuilderInfo &info);

int startTransaction(sqlite3 *db);
int endTransaction(sqlite3 *db);

void storeBlockInRDBMS(struct StoreBasicArgs * basic_args,
                       Block block);

void * consumer_store_transactions(void * params);

#endif  // TOOLS_ETHERBUILDER_INCLUDE_BLOCK_STORE_H_
