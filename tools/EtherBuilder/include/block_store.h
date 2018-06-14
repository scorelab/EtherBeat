/*
 * Created by prabushitha on 6/12/18.
*/
#include <sqlite3.h>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <string>
#include <cstdarg>


static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for(i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void run_sql_query(sqlite3 *db, std::string sql, std::string title="Query Execution") {
    char *zErrMsg = 0;
    int rc;


    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        printf("SQL error (%s): %s\n", title.c_str(), zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        printf("%s successfully\n", title.c_str());
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
             "nonce           INT," \
             "gasPrice           BIGINT," \
             "gasLimit           INT," \
             "receiver           VARCHAR," \
             "value           BIGINT," \
             "v_val           VARCHAR," \
             "r_val           VARCHAR," \
             "s_val           VARCHAR," \
             "init           TEXT" \
             "sender           VARCHAR" \
             "hash            VARCHAR NOT NULL);";

    std::string blocktxTable = "DROP TABLE IF EXISTS Blocktx;" \
    "CREATE TABLE IF NOT EXISTS Blocktx("  \
             "blockId INT NOT NULL," \
             "txId INT NOT NULL," \
             "FOREIGN KEY (blockId) REFERENCES Block(id)," \
             "FOREIGN KEY (txId) REFERENCES Tx(id)," \
             "PRIMARY KEY (blockId, txId));" ;

    std::stringstream ss;
    ss << blockTable << txTable << blocktxTable;
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

std::string createTxSql(Transaction transaction, int blockNumber) {
    std::string block_sql = "INSERT INTO Tx (id, nonce, gasPrice, gasLimit, receiver, value, v_val, r_val, s_val, init, sender, hash ) "  \
         "VALUES (" \
                    "" + std::to_string(blockNumber)+ "," \
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

void storeBlockInRDBMS(sqlite3 *db, Block block){
    std::string db_sql = createBlockSql(block);
    printf("%s\n\n",db_sql.c_str());
    /* Execute SQL statement */
    run_sql_query(db, db_sql, "Block"+std::to_string(block.header.getNumber())+" and Transaction Insertion");

    std::string transactions_sql = "";
    int i;
    for(i=0;i<block.transactions.size(); i++) {
        Transaction transaction = block.transactions[i];
        std::string tx_sql = createTxSql(transaction, block.header.getNumber());
        transactions_sql = transactions_sql+tx_sql;
    }
    // printf("Came here\n\n");
}


