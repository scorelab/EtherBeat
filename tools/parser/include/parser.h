#include "block.h"
#include "account.h"
#include "rlp.h"
#include "leveldb/db.h"
#include <vector>

class Parser {


    leveldb::DB * db;
    uint8_t headerPrefix[1] = {104}; // h : headerPrefix + num (uint64 big endian) + hash -> header
    uint8_t numSuffix[1] = {110}; // n : headerPrefix + num (uint64 big endian) + numSuffix -> hash

    uint8_t blockHashPrefix[1] = {72}; // H : blockHashPrefix + hash -> num (uint64 big endian)

    uint8_t bodyPrefix[1] = {98}; // b : bodyPrefix + num (uint64 big endian) + hash -> block body

    uint8_t blockReceiptsPrefix[1] = {114}; // r : blockReceiptsPrefix + num (uint64 big endian) + hash -> block receipts
    uint8_t lookupPrefix[1] = {108};  // l : lookupPrefix + hash -> transaction/receipt lookup metadata (BlockHash, BlockIndex, Index)

    public:
        Parser(leveldb::DB *);
        Block getBlock(uint64_t blockNumber);
        Block getBlock(std::string blockHashHex);
        TransactionReceipt getTransactionReceipt(std::string transactionHash);


        Account getAccount(std::string address);
        Account getAccount(std::string address, uint64_t blockHeight);

    private:
        Block getBlock(uint64_t blockNumber, std::string blockHash);
        std::vector<TransactionReceipt> getBlockTxReceipts(uint64_t blockNumber, std::string blockHash);

        void updateHeader(Header *header, std::vector<uint8_t> contents, RLP & rlp);
        void updateBody(Block *block, std::vector<uint8_t> contents, RLP & rlp);
        void updateTxReceiptMeta(TransactionReceiptMeta *meta, std::vector<uint8_t> contents, RLP & rlp);
        void updateTxReceipts(std::vector<TransactionReceipt> *receipts, uint64_t blockNumber, std::vector<uint8_t> blockHash, std::vector<uint8_t> contents, RLP & rlp);

        std::vector<uint8_t> createByteVector(std::vector<uint8_t> contents, const RLP & rlp);

        std::string createBlockHashKey(uint64_t blockNumber);
        std::string createBlockHeaderKey(uint64_t blockNumber, std::string blockHash);
        std::string createBlockBodyKey(uint64_t blockNumber, std::string blockHash);
        std::string createBlockReceiptKey(uint64_t blockNumber, std::string blockHash);
        std::string createLookupKey(std::string transactionHash);

};