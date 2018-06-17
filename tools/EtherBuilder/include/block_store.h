/*
 * Created by prabushitha on 6/12/18.
*/
#include <sqlite3.h>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <string>
#include <cstdarg>

struct BuilderInfo {
    std::string INFO_PREFIX_LAST_BLOCKID = "info_last_block_id";
    std::string INFO_PREFIX_LAST_TXID = "info_last_tx_id";
    size_t lastBlockId;
    size_t lastTxId;
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

std::string createTxSql(Transaction transaction, size_t blockId, size_t txId) {
    std::string tx_sql = "INSERT INTO Tx (id, nonce, gasPrice, gasLimit, receiver, value, v_val, r_val, s_val, init, sender, hash ) "  \
         "VALUES (" \
                    "" + std::to_string(txId)+ "," \
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

void storeBlockInRDBMS(sqlite3 *db, struct BuilderInfo &info, Block block){
    std::string db_sql = createBlockSql(block);


    std::string transactions_sql = "";
    int i;
    for(i=0;i<block.transactions.size(); i++) {
        Transaction transaction = block.transactions[i];
        std::string tx_sql = createTxSql(transaction, info.lastBlockId, info.lastTxId);
        transactions_sql = transactions_sql+tx_sql;
        info.lastTxId++;
    }

    /* Execute SQL statement */
    int isQuerySuccess = run_sql_query(db, db_sql+transactions_sql, "Block "+std::to_string(block.header.getNumber())+ " insertion");

    if(isQuerySuccess) {
        info.lastBlockId++;
    } else {
        // Todo : Rollback partial sqlite insertions
    }
    // printf("Came here\n\n");
}


