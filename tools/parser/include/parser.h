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

    public:
        Parser(leveldb::DB *);
        Block getBlock(uint64_t blockNumber);
        Block getBlock(std::string blockHashHex);


        Account getAccount(std::string address);
        Account getAccount(std::string address, uint64_t blockHeight);
    private:
        Block getBlock(uint64_t blockNumber, std::string blockHash);
        void updateHeader(Header *header, std::vector<uint8_t> contents, RLP & rlp);
        void updateBody(Block *block, std::vector<uint8_t> contents, RLP & rlp);

        std::vector<uint8_t> createByteVector(std::vector<uint8_t> contents, const RLP & rlp);
        std::string createBlockHeaderKey(int blockNumber, std::string blockHash);

        std::string createBlockBodyKey(int blockNumber, std::string blockHash);

};