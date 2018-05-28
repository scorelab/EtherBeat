#include "block.h"
#include "account.h"
#include "rlp.h"
#include "leveldb/db.h"
#include <vector>

class Parser {

    leveldb::DB * db;
    uint8_t headerPrefix[1] = {104}; //h
    uint8_t numSuffix[1] = {110}; //n

    uint8_t bodyPrefix[1] = {98}; //b

    public:
        Parser(leveldb::DB *);
        Block getBlock(uint64_t);

        Account getAccount(std::string address);
        Account getAccount(std::string address, uint64_t blockHeight);
    private:
        void updateHeader(Header *header, std::vector<uint8_t> contents, RLP & rlp);
        void updateBody(Block *block, std::vector<uint8_t> contents, RLP & rlp);

        std::vector<uint8_t> createByteVector(std::vector<uint8_t> contents, const RLP & rlp);
        std::string createBlockHeaderKey(int blockNumber, std::string blockHash);

        std::string createBlockBodyKey(int blockNumber, std::string blockHash);

};