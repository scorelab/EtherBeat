/*
 * Created by prabushitha on 6/6/18.
 *
 */
#include <gtest/gtest.h>
#include "extractor.h"


struct ParserTest : testing::Test {
    EtherExtractor* extractor;
    ParserTest() {
        extractor = new EtherExtractor("../test/sample/chaindata");
    }

    ~ParserTest() {
        delete extractor;
    }
};


TEST_F(ParserTest, GetBlockByHash) {
    Block b = extractor->getBlock("0xe01eb24a18d0c117409a7b8946ed16b4c485ed79746644b89250384ed9bf2fda");;
    std::vector<uint8_t> hash;
    EXPECT_EQ(b.hash_bytes.size(), 32);
}
TEST_F(ParserTest, GetBlockByNumber) {
    Block b = extractor->getBlock(189154);;
    std::vector<uint8_t> hash;
    EXPECT_EQ(b.hash_bytes.size(), 32);
    EXPECT_EQ(b.transactions.size(), 16);
    EXPECT_EQ(b.ommerHashes_bytes.size(), 0);
    EXPECT_EQ(b.header.nonce_bytes.size(), 8);
}
TEST_F(ParserTest, GetTransactionReceipt) {
    TransactionReceipt tr = extractor->getTransactionReceipt("0xfddd30b69b0c4f918785e83c8b8ddd21fe3e2019a142fc382b88fa5de6c88fa2");
    EXPECT_EQ(tr.blockNumber, 189154);
    EXPECT_EQ(tr.transactionIndex, 0);
}