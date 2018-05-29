#include "leveldb/db.h"
#include "parser.h"
#include "utils.h"
#include <iostream>

int main(int argc, const char *argv[]) {
    // connect to leveldb
    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "/home/prabushitha/.ethereum/rinkeby/geth/chaindata", &db);

    // when error occured while connecting to db
    if (!status.ok()){
        std::cerr << status.ToString() << std::endl;
        delete db;
        return 0;
    }

    Parser parser(db);
    /*
     * // TESTING PURPOSE
    int count = 0;
    int i;
    for(i=1;i<200000;i++) {
        Block b = parser.getBlock(i);
        // if (!b.transactions.empty()) break;
        count = count+b.transactions.size();
    }
    printf("BLOCK = %d, TOTAL TRANSACTIONS = %d\n", i, count);
    */

    Block b = parser.getBlock("0xdb16f0d4465f2fd79f10ba539b169404a3e026db1be082e7fd6071b4c5f37db7");
    b.print();

    // Block b = parser.getBlock(189154);
    // b.print();
    // Block b = parser.getBlock(2267598);


    // testing account
    /*
     * // TESTING PURPOSE
    if(!b.transactions.empty()) {
        std::string addr = hexStr((unsigned char *)&b.transactions[0].from[0], b.transactions[0].from.size());
        Account acc = parser.getAccount(addr);

    }*/



    delete db;
    return 1;
}
