/*
 * Created by prabushitha on 7/1/18.
 * Copyright [2018] <ScoreLab Organization>
*/
#include "block_store.h"
#include "rocksdb/db.h"
#include <sqlite3.h>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <string>
#include <cstdarg>

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int run_sql_query(sqlite3 *db, std::string sql, std::string title = "Query Execution") {
    char *zErrMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), callback, nullptr, &zErrMsg);

    if ( rc != SQLITE_OK ) {
        printf("SQL error (%s): %s\n", title.c_str(), zErrMsg);
        sqlite3_free(zErrMsg);
        return 0;
    } else {
        return 1;
    }
}

void createRDBMSSchema(sqlite3 *db) {
    char *zErrMsg = 0;
    int rc;

    std::string blockTable = "DROP TABLE IF EXISTS block;"\
    "CREATE TABLE IF NOT EXISTS block("  \
             "id INT PRIMARY KEY     NOT NULL," \
             "hash           VARCHAR    NOT NULL," \
             "parentHash           VARCHAR    NOT NULL," \
             "sha3Uncles           VARCHAR," \
             "beneficiary          VARCHAR," \
             "stateRoot           VARCHAR," \
             "transactionsRoot    VARCHAR," \
             "receiptsRoot        VARCHAR," \
             "logsBloom        TEXT," \
             "difficulty        INT," \
             "gasLimit            INT ," \
             "gasUsed            INT," \
             "timestamp            DATETIME ," \
             "extraData            TEXT ," \
             "mixHash            VARCHAR ," \
             "nonce            VARCHAR );";

    std::string txTable = "DROP TABLE IF EXISTS tx;" \
    "CREATE TABLE IF NOT EXISTS tx("  \
             "id INT PRIMARY KEY     NOT NULL," \
             "tx_type          VARCHAR," \
             "nonce           INT," \
             "gasPrice        REAL," \
             "gasLimit        INT," \
             "receiver        VARCHAR," \
             "value           REAL," \
             "v_val           VARCHAR," \
             "r_val           VARCHAR," \
             "s_val           VARCHAR," \
             "init            TEXT," \
             "sender          VARCHAR," \
             "hash            VARCHAR NOT NULL);";

    std::string txReceiptTable = "DROP TABLE IF EXISTS txreceipt;" \
    "CREATE TABLE IF NOT EXISTS txreceipt("  \
             "id INT PRIMARY KEY     NOT NULL," \
             "txHash          VARCHAR NOT NULL," \
             "blockHash          VARCHAR NOT NULL," \
             "blockNumber           INT," \
             "transactionIndex        INT," \
             "status        VARCHAR," \
             "gasUsed        INT," \
             "cumulativeGasUsed           REAL," \
             "contractAddress           VARCHAR," \
             "logsBloom           VARCHAR);";


    std::string blocktxTable = "DROP TABLE IF EXISTS blocktx;" \
    "CREATE TABLE IF NOT EXISTS blocktx("  \
             "blockId INT NOT NULL," \
             "txId INT NOT NULL," \
             "FOREIGN KEY (blockId) REFERENCES Block(id)," \
             "FOREIGN KEY (txId) REFERENCES Tx(id)," \
             "PRIMARY KEY (blockId, txId));";

    std::string accountTxTable = "DROP TABLE IF EXISTS fromto;" \
    "CREATE TABLE IF NOT EXISTS fromto("  \
             "sender INT NOT NULL," \
             "receiver INT NOT NULL," \
             "amount REAL," \
             "txId INT);";

    std::string accountTable = "DROP TABLE IF EXISTS account;" \
    "CREATE TABLE IF NOT EXISTS account("  \
             "id INT PRIMARY KEY     NOT NULL," \
             "address INT NOT NULL," \
             "isContract INT NOT NULL," \
             "amount REAL);";

    std::stringstream ss;
    ss << blockTable << txTable << txReceiptTable << blocktxTable << accountTxTable;
    std::string sql = ss.str();

    /* Execute SQL statement */
    run_sql_query(db, sql, "Table Creation");
}

void bindToBlockSql(sqlite3_stmt * stmt, Block b) {
    std::string hash = b.getHash();
    std::string parentHash = b.header.getParentHash();
    std::string sha3Uncles = b.header.getSha3Uncles();
    std::string beneficiary = b.header.getBeneficiary();
    std::string stateRoot = b.header.getStateRoot();
    std::string transactionsRoot = b.header.getTransactionsRoot();
    std::string receiptsRoot = b.header.getReceiptsRoot();
    std::string logsBloom = b.header.getLogsBloom();
    std::string extraData = b.header.getExtraData();
    std::string mixHash = b.header.getMixHash();
    std::string nonce = b.header.getNonce();

    sqlite3_bind_int64(stmt, 1, b.header.getNumber());
    sqlite3_bind_text(stmt, 2,  hash.c_str(), strlen(hash.c_str()), 0);
    sqlite3_bind_text(stmt, 3,  parentHash.c_str(), strlen(parentHash.c_str()), 0);
    sqlite3_bind_text(stmt, 4,  sha3Uncles.c_str(), strlen(sha3Uncles.c_str()), 0);
    sqlite3_bind_text(stmt, 5,  beneficiary.c_str(), strlen(beneficiary.c_str()), 0);
    sqlite3_bind_text(stmt, 6,  stateRoot.c_str(), strlen(stateRoot.c_str()), 0);
    sqlite3_bind_text(stmt, 7,  transactionsRoot.c_str(), strlen(transactionsRoot.c_str()), 0);
    sqlite3_bind_text(stmt, 8,  receiptsRoot.c_str(), strlen(receiptsRoot.c_str()), 0);
    sqlite3_bind_text(stmt, 9,  logsBloom.c_str(), strlen(logsBloom.c_str()), 0);

    sqlite3_bind_int(stmt, 10, b.header.getDifficulty());
    sqlite3_bind_int(stmt, 11, b.header.getGasLimit());
    sqlite3_bind_int(stmt, 12, b.header.getGasUsed());
    sqlite3_bind_double(stmt, 13, static_cast<double>(b.header.getTimestamp()));

    sqlite3_bind_text(stmt, 14,  extraData.c_str(), strlen(extraData.c_str()), 0);
    sqlite3_bind_text(stmt, 15,  mixHash.c_str(), strlen(mixHash.c_str()), 0);
    sqlite3_bind_text(stmt, 16,  nonce.c_str(), strlen(nonce.c_str()), 0);

    sqlite3_step(stmt);
    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);
}

void bindToTxSql(sqlite3_stmt * stmt, Transaction transaction, size_t blockId, size_t txId) {
    std::string type = transaction.getType();
    std::string to = transaction.getTo();
    std::string v = transaction.getV();
    std::string r = transaction.getR();
    std::string s = transaction.getS();
    std::string data = transaction.getData();
    std::string from = transaction.getFrom();
    std::string hash = transaction.getHash();

    sqlite3_bind_int64(stmt, 1, txId);
    sqlite3_bind_text(stmt, 2,  type.c_str(), strlen(type.c_str()), 0);

    sqlite3_bind_int(stmt, 3, transaction.getNonce());
    sqlite3_bind_double(stmt, 4,  transaction.getValue());
    sqlite3_bind_int(stmt, 5,  transaction.getGasLimit());

    sqlite3_bind_text(stmt, 6,  to.c_str(), strlen(to.c_str()), 0);
    sqlite3_bind_text(stmt, 7,  type.c_str(), strlen(type.c_str()), 0);
    sqlite3_bind_text(stmt, 8,  v.c_str(), strlen(v.c_str()), 0);
    sqlite3_bind_text(stmt, 9,  r.c_str(), strlen(r.c_str()), 0);
    sqlite3_bind_text(stmt, 10,  s.c_str(), strlen(s.c_str()), 0);
    sqlite3_bind_text(stmt, 11,  data.c_str(), strlen(data.c_str()), 0);
    sqlite3_bind_text(stmt, 12,  from.c_str(), strlen(from.c_str()), 0);
    sqlite3_bind_text(stmt, 13,  hash.c_str(), strlen(hash.c_str()), 0);


    sqlite3_step(stmt);
    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);
}

void bindToBlockTxSql(sqlite3_stmt * stmt, size_t blockId, size_t txId) {
    sqlite3_bind_int64(stmt, 1, blockId);
    sqlite3_bind_int64(stmt, 2, txId);

    sqlite3_step(stmt);
    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);
}

void bindToTxReceiptSql(sqlite3_stmt * stmt, TransactionReceipt receipt, size_t txId) {
    std::string txhash =  receipt.getTransactionHash();
    std::string blockhash =  receipt.getBlockHash();
    std::string status =  receipt.getStatus();
    std::string contractaddress =  receipt.getContractAddress();
    std::string logsbloom =  receipt.getLogsBloom();

    sqlite3_bind_int64(stmt, 1, txId);
    sqlite3_bind_text(stmt, 2,  txhash.c_str(), strlen(txhash.c_str()), 0);
    sqlite3_bind_text(stmt, 3,  blockhash.c_str(), strlen(blockhash.c_str()), 0);
    sqlite3_bind_int64(stmt, 4, receipt.getBlockNumber());
    sqlite3_bind_int(stmt, 5, receipt.getTransactionIndex());
    sqlite3_bind_text(stmt, 6,  status.c_str(), strlen(status.c_str()), 0);
    sqlite3_bind_int(stmt, 7, receipt.getGasUsed());
    sqlite3_bind_double(stmt, 8, receipt.getCumulativeGasUsed());
    sqlite3_bind_text(stmt, 9,  contractaddress.c_str(), strlen(contractaddress.c_str()), 0);
    sqlite3_bind_text(stmt, 10,  logsbloom.c_str(), strlen(logsbloom.c_str()), 0);

    sqlite3_step(stmt);
    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);
}

void bindToFromtoSql(sqlite3_stmt * stmt, size_t from, size_t to, double amount, size_t txid) {
    sqlite3_bind_int64(stmt, 1, from);
    sqlite3_bind_int64(stmt, 2, to);
    sqlite3_bind_double(stmt, 3, amount);
    sqlite3_bind_int64(stmt, 4, txid);


    sqlite3_step(stmt);
    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);
}

size_t getHashId(rocksdb::DB* db_rocks, std::string hash) {
    std::string existingId;
    rocksdb::Status status = db_rocks->Get(rocksdb::ReadOptions(), hash, &existingId);

    if (status.ok()) {
        return (size_t)std::stoi( existingId );
    }

    return 0;
}

bool updateHashId(rocksdb::DB* db_rocks, std::string hash, int id) {
    rocksdb::Status status = db_rocks->Put(rocksdb::WriteOptions(), hash, std::to_string(id));
    return status.ok();
}

bool isAddressValid(std::string address) {
    // check if address is valid (present)
    return !address.empty();
}

size_t updateAndGetAccountHashId(rocksdb::DB* db_rocks, std::string hash, struct BuilderInfo &info) {
    size_t id = 0;
    if (isAddressValid(hash)) {
        hash = info.PREFIX_ADDRESS + hash;
        id = getHashId(db_rocks, hash);

        if (id == 0) {
            // no id is associated with this hash
            id = info.nextAddressId;
            updateHashId(db_rocks, hash, id);
            info.nextAddressId++;
        }
    }
    return id;
}

int startTransaction(sqlite3 *db) {
    char* errorMessage;
    return sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, &errorMessage);
}
int endTransaction(sqlite3 *db) {
    char* errorMessage;
    return sqlite3_exec(db, "COMMIT TRANSACTION", nullptr, nullptr, &errorMessage);
}

void storeBlockInRDBMS(struct StoreBasicArgs * basic_args, Block block) {
    /*
     *  Block sqlite and rocksdb
     */
    bindToBlockSql(basic_args->stmt_block, block);

    // rocksdb : block hash -> id mapping
    updateHashId(basic_args->db_rocks, basic_args->info.PREFIX_BLOCK+block.getHash(), basic_args->info.nextBlockId);

    /*
     *  Transactions and tx receipt to sqlite and rocksdb
     */

    for (const auto &transaction : block.transactions) {
        pthread_mutex_lock(&(basic_args->txbuffer->mutex));

        // check whether if the buffer is full
        if (basic_args->txbuffer->occupied >= TX_BUFFER_SIZE) {
            // wait till consumer process a transaction
            pthread_cond_wait(&(basic_args->txbuffer->wait_on_full_tx), &(basic_args->txbuffer->mutex));
        }

        // add a transaction to the buffer
        basic_args->txbuffer->buffer[basic_args->txbuffer->next_in] = transaction;
        basic_args->txbuffer->next_in = (basic_args->txbuffer->next_in+1)%TX_BUFFER_SIZE;
        basic_args->txbuffer->occupied++;

        // signal for consumer if it's waiting on empty buffer
        pthread_cond_signal(&(basic_args->txbuffer->wait_on_no_tx));
        pthread_mutex_unlock(&(basic_args->txbuffer->mutex));
    }

    basic_args->info.nextBlockId++;

    // Todo : transaction roll backs on failure
}

void * consumer_store_transactions(void * params) {
    /*
     * This is the worker function (thread) - To store transactions and accounts.
     *
     * It's NOT SAFE to use the following attributes of 'struct BuilderInfo' inside this method
     * -> nextBlockId
     *
     * Reason is nextBlockId is updated by master thread and may not reflect the block id of the transaction processing in this method
     *
     * To get the block id use TransactionReceipt's block number
     */
    auto * basic_args = (struct StoreBasicArgs *)params;

    while ((!(*(basic_args->isMasterOver))) || (*(basic_args->isMasterOver) && basic_args->txbuffer->occupied > 0) ) {
        pthread_mutex_lock(&(basic_args->txbuffer->mutex));

        // check whether if there's new transactions added to the buffer
        if (basic_args->txbuffer->occupied <= 0) {
            // wait till a transaction is added to the buffer
            pthread_cond_wait(&(basic_args->txbuffer->wait_on_no_tx), &(basic_args->txbuffer->mutex));
        }

        if (basic_args->txbuffer->occupied == 0) {
            pthread_mutex_unlock(&(basic_args->txbuffer->mutex));
            break;
        }
        // get the next transaction from the buffer to process
        Transaction transaction = basic_args->txbuffer->buffer[basic_args->txbuffer->next_out];
        basic_args->txbuffer->next_out = (basic_args->txbuffer->next_out + 1) % TX_BUFFER_SIZE;
        basic_args->txbuffer->occupied--;

        // signal for producer if it's waiting on the full buffer
        pthread_cond_signal(&(basic_args->txbuffer->wait_on_full_tx));
        pthread_mutex_unlock(&(basic_args->txbuffer->mutex));

        TransactionReceipt receipt = basic_args->extractor.getTransactionReceipt(transaction.getHash());

        size_t block_id = receipt.getBlockNumber();

        bindToTxSql(basic_args->stmt_tx, transaction, block_id, basic_args->info.nextTxId);
        bindToBlockTxSql(basic_args->stmt_blocktx, block_id, basic_args->info.nextTxId);
        bindToTxReceiptSql(basic_args->stmt_txreceipt, receipt, basic_args->info.nextTxId);

        // rocksdb : transaction hash -> id mapping
        updateHashId(basic_args->db_rocks, basic_args->info.PREFIX_TX+transaction.getHash(), basic_args->info.nextTxId);

        // rocksdb : address -> id mapping (get existing id if exists, otherwise new id will be created)
        size_t senderId = updateAndGetAccountHashId(basic_args->db_rocks, transaction.getFrom(), basic_args->info);
        size_t receiverId = updateAndGetAccountHashId(basic_args->db_rocks, transaction.getTo(), basic_args->info);

        // if there's a sender and a receiver for a transaction, should insert to fromto table
        if (senderId > 0 && receiverId > 0) {
            // sql string to insert from-to
            bindToFromtoSql(basic_args->stmt_fromto, senderId, receiverId, transaction.getValue(), basic_args->info.nextTxId);
        } else {
            // NOT A NORMAL TRANSACTION
        }

        basic_args->info.nextTxId++;
    }
    return params;
}
