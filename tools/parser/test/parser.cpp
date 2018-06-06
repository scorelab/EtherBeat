/*
 * Created by prabushitha on 6/6/18.
 *
 */
#include <gtest/gtest.h>
#include "leveldb/db.h"
#include "parser.h"


struct ParserTest : testing::Test {
    Parser* parser;
    leveldb::DB* db;
    ParserTest() {
        leveldb::Options options;
        options.create_if_missing = true;
        leveldb::Status status = leveldb::DB::Open(options, "../test/sample/chaindata", &db);

        parser = new Parser(db);
    }

    ~ParserTest() {
        delete parser;
        delete db;
    }
};


TEST_F(ParserTest, GetBlockByHash) {
    Block b = parser->getBlock("0xe01eb24a18d0c117409a7b8946ed16b4c485ed79746644b89250384ed9bf2fda");;
    std::vector<uint8_t> hash;
    EXPECT_EQ(b.hash.size(), 32);
}
TEST_F(ParserTest, GetBlockByNumber) {
    Block b = parser->getBlock(189154);;
    std::vector<uint8_t> hash;
    EXPECT_EQ(b.hash.size(), 32);
    EXPECT_EQ(b.transactions.size(), 16);
    EXPECT_EQ(b.ommerHashes.size(), 0);
    EXPECT_EQ(b.header.nonce.size(), 8);
}
TEST_F(ParserTest, GetTransactionReceipt) {
    TransactionReceipt tr = parser->getTransactionReceipt("0xfddd30b69b0c4f918785e83c8b8ddd21fe3e2019a142fc382b88fa5de6c88fa2");
    EXPECT_EQ(tr.blockNumber, 189154);
    EXPECT_EQ(tr.transactionIndex, 0);
}