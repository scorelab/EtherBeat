/*
 * Created by prabushitha on 6/29/18.
 *
 * This code is to check the performance of SQLite with different configurations.
 * This file is not contributing to the functionality of the EtherBuilder
 *
*/
#include "block_store.h"
#include <sqlite3.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <chrono>

#define NUMBER_OF_INSERTIONS 100000

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for(i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

long int getTime() {

    auto ms = std::chrono::duration_cast< std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );

    return ms.count();
}

void createResearchdb(sqlite3 *db) {
    std::string researchTable = "DROP TABLE IF EXISTS research_tbl;" \
    "CREATE TABLE IF NOT EXISTS research_tbl("  \
             "col1 VARCHAR," \
             "col2 VARCHAR," \
             "col3 VARCHAR);" ;
    run_sql_query(db, researchTable, "Research Table Creation");
}

void planeInsertions(sqlite3 *db) {
    createResearchdb(db);
    long int startTime = getTime();

    char *zErrMsg = 0;

    std::string sql = "";
    int i;
    for(i=0;i<NUMBER_OF_INSERTIONS; i++) {
        sql = sql + "INSERT INTO research_tbl (col1, col2, col3) VALUES ('a_"+ std::to_string(i+1)+"','b','c');";
    }


    sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

    long int endTime = getTime();
    std::cout << "time plane = " << endTime-startTime << " ms" << std::endl;

}

void transactionInsertions(sqlite3 *db) {
    createResearchdb(db);

    long int startTime = getTime();

    char *zErrMsg = 0;
    int rc;


    sqlite3_exec(db, "PRAGMA cache_size=10000", NULL, NULL, &zErrMsg);
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &zErrMsg);

    int i;
    for(i=0;i<NUMBER_OF_INSERTIONS; i++) {
        std::string sql = "INSERT INTO research_tbl (col1, col2, col3) VALUES ('a_"+ std::to_string(i+1)+"','b','c');";
        sqlite3_exec(db, sql.c_str(), NULL, NULL, &zErrMsg);
    }
    sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &zErrMsg);

    long int endTime = getTime();
    std::cout << "time transactions = " << endTime-startTime << " ms" << std::endl;
}



void transactionsWithOptionsInsertions(sqlite3 *db) {
    createResearchdb(db);

    long int startTime = getTime();

    char *zErrMsg = 0;
    int rc;

    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &zErrMsg);

    int i;
    for(i=0;i<NUMBER_OF_INSERTIONS; i++) {
        std::string sql = "INSERT INTO research_tbl (col1, col2, col3) VALUES ('a_"+ std::to_string(i+1)+"','b','c');";
        sqlite3_exec(db, sql.c_str(), NULL, NULL, &zErrMsg);
    }
    sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &zErrMsg);

    long int endTime = getTime();
    std::cout << "time transactions (+options) = " << endTime-startTime << " ms" << std::endl;
}

int transactionsWithPrepared(sqlite3 *db) {
    createResearchdb(db);

    long int startTime = getTime();

    char* errorMessage;

    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);

    char const *szSQL = "INSERT INTO research_tbl (col1, col2, col3) VALUES (?,?,?);";
    sqlite3_stmt * pStmt = nullptr;
    int rc = sqlite3_prepare(db, szSQL, -1, &pStmt, nullptr);

    if( rc == SQLITE_OK ) {
        for(int x=0;x<NUMBER_OF_INSERTIONS;x++) {
            // bind the value
            std::string col1 = "a_"+std::to_string(x+1);
            std::string col2 = "b";
            std::string col3 = "c";

            sqlite3_bind_text(pStmt, 1,  col1.c_str(), strlen(col1.c_str()), 0);
            sqlite3_bind_text(pStmt, 2,  col2.c_str(), strlen(col2.c_str()), 0);
            sqlite3_bind_text(pStmt, 3,  col3.c_str(), strlen(col3.c_str()), 0);

            if (sqlite3_step(pStmt) != SQLITE_DONE) {
                return 1;
            }
            sqlite3_reset(pStmt);
            sqlite3_clear_bindings(pStmt);
        }
        sqlite3_finalize(pStmt);
        sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
    } else {
        fprintf(stderr, "SQL error: %s\n", errorMessage);
        sqlite3_free(errorMessage);
    }

    long int endTime = getTime();
    std::cout << "time transactions with prepared = " << endTime-startTime << " ms" << std::endl;
    return 2;
}

int main() {
    // Connect to SQLite
    sqlite3 *db_sqlite;
    int rc = sqlite3_open("/tmp/dbsqlite/builder_research.db", &db_sqlite);

    if (rc) {
        fprintf(stderr, "SQLite db failed : %s\n", sqlite3_errmsg(db_sqlite));
        return (0);
    } else {
        fprintf(stderr, "SQLite db connected\n");
    }

    transactionsWithPrepared(db_sqlite);

}


