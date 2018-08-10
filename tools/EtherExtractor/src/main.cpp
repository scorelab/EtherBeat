/*
 * Created by prabushitha on 5/6/18.
 * Copyright [2018] <ScoreLab Organization>
 *
*/

#include "extractor.h"
#include <iostream>
#include <chrono>

int main(int argc, const char *argv[]) {
    /*
     *  THIS IS A SAMPLE MAIN FILE
     *  If you're developing an Standalone Application 'ETHER EXTRACTOR',
     *  you may need to change this.
     */

    EtherExtractor extractor("/home/prabushitha/.ethereum/rinkeby/geth/chaindata");

    Block b = extractor.getBlock(189154);
    b.print();

    TransactionReceipt tr = extractor.getTransactionReceipt("0xfddd30b69b0c4f918785e83c8b8ddd21fe3e2019a142fc382b88fa5de6c88fa2");
    tr.print();

    return 1;
}
