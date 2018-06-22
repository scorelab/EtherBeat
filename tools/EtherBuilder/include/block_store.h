/*
 * Created by prabushitha on 6/12/18.
*/
#include "rocksdb/db.h"
#include <sqlite3.h>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <string>
#include <cstdarg>


struct BuilderInfo {
    std::string INFO_PREFIX_LAST_BLOCKID = "info_last_block_id";
    std::string INFO_PREFIX_LAST_TXID = "info_last_tx_id";
    std::string INFO_PREFIX_LAST_ADDRESSID = "info_last_address_id";

    size_t lastBlockId;
    size_t lastTxId;
    size_t lastAddressId;

    std::string PREFIX_BLOCK = "block_";
    std::string PREFIX_TX = "tx_";
    std::string PREFIX_ADDRESS = "address_";
};

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for(i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int run_sql_query(sqlite3 *db, std::string sql, std::string title="Query Execution") {
    char *zErrMsg = 0;
    int rc;


    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        printf("SQL error (%s): %s\n", title.c_str(), zErrMsg);
        sqlite3_free(zErrMsg);
        return 0;
    } else {
        // printf("%s successful\n", title.c_str());
        return 1;
    }
}

void createRDBMSSchema(sqlite3 *db) {
    // sqlite> PRAGMA foreign_keys = ON;
    char *zErrMsg = 0;
    int rc;

    std::string blockTable = "DROP TABLE IF EXISTS Block;"\
    "CREATE TABLE IF NOT EXISTS Block("  \
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

    std::string txTable = "DROP TABLE IF EXISTS Tx;" \
    "CREATE TABLE IF NOT EXISTS Tx("  \
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

    std::string txReceiptTable = "DROP TABLE IF EXISTS Txreceipt;" \
    "CREATE TABLE IF NOT EXISTS Txreceipt("  \
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


    std::string blocktxTable = "DROP TABLE IF EXISTS Blocktx;" \
    "CREATE TABLE IF NOT EXISTS Blocktx("  \
             "blockId INT NOT NULL," \
             "txId INT NOT NULL," \
             "FOREIGN KEY (blockId) REFERENCES Block(id)," \
             "FOREIGN KEY (txId) REFERENCES Tx(id)," \
             "PRIMARY KEY (blockId, txId));" ;

    std::string accountTxTable = "DROP TABLE IF EXISTS Accounttx;" \
    "CREATE TABLE IF NOT EXISTS Accounttx("  \
             "senderId INT NOT NULL," \
             "receiverId INT NOT NULL," \
             "amount REAL," \
             "txId INT);" ;



    std::stringstream ss;
    ss << blockTable << txTable << txReceiptTable << blocktxTable << accountTxTable;
    std::string sql = ss.str();

    /* Execute SQL statement */
    run_sql_query(db, sql, "Table Creation");
}

std::string createBlockSql(Block b) {

    std::string block_sql = "INSERT INTO Block (id, hash, parentHash, sha3Uncles, beneficiary, stateRoot, transactionsRoot, receiptsRoot, logsBloom, difficulty, gasLimit, gasUsed, timestamp, extraData, mixHash, nonce ) "  \
         "VALUES (" \
                    "" + std::to_string(b.header.getNumber())+ "," \
                    "'"+ b.getHash()+ "'," \
                    "'"+ b.header.getParentHash()+ "'," \
                    "'"+ b.header.getSha3Uncles()+ "'," \
                    "'"+ b.header.getBeneficiary()+ "'," \
                    "'"+ b.header.getStateRoot()+ "'," \
                    "'"+ b.header.getTransactionsRoot()+ "'," \
                    "'"+ b.header.getReceiptsRoot()+ "'," \
                    "'"+ b.header.getLogsBloom()+ "'," \
                    "" + std::to_string(b.header.getDifficulty())+ "," \
                    "" + std::to_string(b.header.getGasLimit())+ "," \
                    "" + std::to_string(b.header.getGasUsed())+ "," \
                    "datetime("+ std::to_string(b.header.getTimestamp())+ ", 'unixepoch', 'localtime')," \
                    "'"+ b.header.getExtraData()+ "'," \
                    "'"+ b.header.getMixHash()+ "'," \
                    "'"+ b.header.getNonce() + "'" \
                  "); " ;

    return block_sql;
}

std::string createTxSql(Transaction transaction, size_t blockId, size_t txId) {
    std::string tx_sql = "INSERT INTO Tx (id, tx_type, nonce, gasPrice, gasLimit, receiver, value, v_val, r_val, s_val, init, sender, hash ) "  \
         "VALUES (" \
                    "" + std::to_string(txId)+ "," \
                    "'"+ transaction.getType()+ "'," \
                    ""+ std::to_string(transaction.getNonce())+ "," \
                    ""+  std::to_string(transaction.getGasPrice())+ "," \
                    ""+ std::to_string(transaction.getGasLimit())+ "," \
                    "'"+ transaction.getTo()+ "'," \
                    ""+  std::to_string(transaction.getValue())+ "," \
                    "'"+ transaction.getV()+ "'," \
                    "'"+ transaction.getR()+ "'," \
                    "'"+ transaction.getS()+ "'," \
                    "'"+ transaction.getData()+ "'," \
                    "'"+ transaction.getFrom()+ "'," \
                    "'"+ transaction.getHash()+ "'" \
                  "); " ;
    std::string blocktx_sql = "INSERT INTO Blocktx (blockId, txId) "  \
         "VALUES (" \
            "" + std::to_string(blockId)+ "," \
            "" + std::to_string(txId)+ "" \
         "); " ;

    return tx_sql+blocktx_sql;
}

std::string createTxReceiptSql(TransactionReceipt receipt, size_t txId) {
    std::string receipt_sql = "INSERT INTO Txreceipt (id , txHash, blockHash, blockNumber, transactionIndex, status, gasUsed, cumulativeGasUsed, contractAddress, logsBloom) "  \
         "VALUES (" \
                    "" + std::to_string(txId)+ "," \
                    "'"+ receipt.getTransactionHash()+ "'," \
                    "'"+ receipt.getBlockHash()+ "'," \
                    ""+ std::to_string(receipt.getBlockNumber())+ "," \
                    ""+  std::to_string(receipt.getTransactionIndex())+ "," \
                    "'"+ receipt.getStatus() + "'," \
                    ""+  std::to_string(receipt.getGasUsed())+ "," \
                    ""+  std::to_string(receipt.getCumulativeGasUsed())+ "," \
                    "'"+ receipt.getContractAddress()+ "'," \
                    "'"+ receipt.getLogsBloom()+ "'); " ;

    return receipt_sql;
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
    if (status.ok()) {
        return true;
    }

    return false;
}

size_t updateAndGetAccountHashId(rocksdb::DB* db_rocks, std::string hash, struct BuilderInfo &info) {
    hash = info.PREFIX_ADDRESS+hash;
    size_t id = getHashId(db_rocks, hash);

    if ( id == 0) {
        id = info.lastAddressId;
        updateHashId(db_rocks, hash, id);
        info.lastAddressId++;
    }

    return id;

}


void storeBlockInRDBMS(sqlite3 *db, rocksdb::DB* db_rocks, struct BuilderInfo &info, Parser &parser, Block block) { //

    std::string db_sql = createBlockSql(block);

    // block hash -> id mapping
    updateHashId(db_rocks, info.PREFIX_BLOCK+block.getHash(), info.lastBlockId);

    std::string transactions_sql = "";
    std::string receipts_sql = "";
    int i;
    for(i=0;i<block.transactions.size(); i++) {
        Transaction transaction = block.transactions[i];
        std::string tx_sql = createTxSql(transaction, info.lastBlockId, info.lastTxId);
        transactions_sql = transactions_sql+tx_sql;

        TransactionReceipt receipt = parser.getTransactionReceipt(transaction.getHash());
        std::string tx_receipt_sql = createTxReceiptSql(receipt, info.lastTxId);
        receipts_sql = receipts_sql+tx_receipt_sql;

        // transaction hash -> id mapping
        updateHashId(db_rocks, info.PREFIX_TX+transaction.getHash(), info.lastTxId);

        // address -> id mapping
        size_t senderId = updateAndGetAccountHashId(db_rocks, transaction.getFrom(), info);
        size_t receiverId = updateAndGetAccountHashId(db_rocks, transaction.getTo(), info);

        info.lastTxId++;
    }

    // Execute SQL statement
    int isQuerySuccess = run_sql_query(db, db_sql+transactions_sql+receipts_sql, "Block "+std::to_string(block.header.getNumber())+ " insertion");

    if(isQuerySuccess) {
        info.lastBlockId++;
    } else {
        // Todo : Rollback partial sqlite insertions
    }
    // printf("Came here\n\n");
}
