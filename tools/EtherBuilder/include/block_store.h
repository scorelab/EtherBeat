/*
 * Created by prabushitha on 6/12/18.
*/
#include "parser.h"
#include "rocksdb/db.h"
#include <sqlite3.h>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <string>
#include <cstdarg>



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


int run_sql_query(sqlite3 *db, std::string sql, std::string title);

void createRDBMSSchema(sqlite3 *db);
void bindToBlockSql(sqlite3_stmt * stmt, Block b);

void bindToTxSql(sqlite3_stmt * stmt, Transaction transaction, size_t blockId, size_t txId);

void bindToBlockTxSql(sqlite3_stmt * stmt, size_t blockId, size_t txId);

void bindToTxReceiptSql(sqlite3_stmt * stmt, TransactionReceipt receipt, size_t txId);

void bindToFromtoSql(sqlite3_stmt * stmt, size_t from, size_t to, double amount, size_t txid);

size_t getHashId(rocksdb::DB* db_rocks, std::string hash);
bool updateHashId(rocksdb::DB* db_rocks, std::string hash, int id);

bool isAddressValid(std::string address);

size_t updateAndGetAccountHashId(rocksdb::DB* db_rocks, std::string hash, struct BuilderInfo &info);

int startTransaction(sqlite3 *db);
int endTransaction(sqlite3 *db);

void storeBlockInRDBMS(sqlite3_stmt * stmt_block,
                       sqlite3_stmt * stmt_tx,
                       sqlite3_stmt * stmt_blocktx,
                       sqlite3_stmt * stmt_txreceipt,
                       sqlite3_stmt * stmt_fromto,
                       rocksdb::DB* db_rocks,
                       struct BuilderInfo &info,
                       Parser &parser,
                       Block block);

