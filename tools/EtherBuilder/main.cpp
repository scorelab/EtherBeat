#include <iostream>
#include "leveldb/db.h"
#include "parser.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "/home/prabushitha/.ethereum/rinkeby/geth/chaindata", &db);

    Parser p(db);
    Block b = p.getBlock(100);
    b.print();
    return 0;
}