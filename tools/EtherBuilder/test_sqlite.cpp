/*
 * Created by prabushitha on 6/29/18.
*/
#include "parser.h"
#include "test_sqlite.h"
#include <sqlite3.h>
#include <iostream>
#include <stdio.h>


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

    // transactionInsertions(db_sqlite);
    // planeInsertions(db_sqlite);
    // transactionsWithOptionsInsertions(db_sqlite);
    transactionsWithPrepared(db_sqlite);



}


