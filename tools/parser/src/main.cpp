/*
 * Created by prabushitha on 5/6/18.
*/
#include "leveldb/db.h"
#include "parser.h"
#include <iostream>
#include <chrono>

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
    // TESTING PURPOSE
    /*
    std::chrono::milliseconds ms1 = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );
    int count = 0;
    int i;
    for(i=1;i<200000;i++) {
        Block b = parser.getBlock(i);

        int j;
        for(j=0;j<b.transactions.size();j++){
            std::string txHash = hexStr(&b.transactions[j].hash[0], (int)b.transactions[j].hash.size());
            TransactionReceipt tr = parser.getTransactionReceipt(txHash);
        }
        count = count+b.transactions.size();
    }
    std::chrono::milliseconds ms2 = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );
    printf("BLOCK = %d, TOTAL TRANSACTIONS = %d\n", i, count);

    std::cout << (ms2.count()-ms1.count())/1000 << std::endl;
    */

    // Block b = parser.getBlock("0xe01eb24a18d0c117409a7b8946ed16b4c485ed79746644b89250384ed9bf2fda");
    // Block b2 = parser.getBlock("0xdb16f0d4465f2fd79f10ba539b169404a3e026db1be082e7fd6071b4c5f37db7");
    // b2.print();

    Block b = parser.getBlock(189154);
    // Block b = parser.getBlock(1);
    b.print();
    // Block b = parser.getBlock(2267598);
    // Block b = parser.getBlock(1795026);
    // b.print();

    TransactionReceipt tr = parser.getTransactionReceipt("0xfddd30b69b0c4f918785e83c8b8ddd21fe3e2019a142fc382b88fa5de6c88fa2");
    tr.print();
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
