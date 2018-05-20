#include "block.h"
#include "rlp.h"
#include "leveldb/db.h"
#include <vector>

class Parser {

    leveldb::DB * db;
    uint8_t headerPrefix[1] = {104};
    uint8_t numSuffix[1] = {110};

    public:
        Parser(leveldb::DB *);
        Block getBlock(uint64_t);
    private:
        Header createHeader(std::vector<uint8_t> content, RLP & rlp);
        std::vector<uint8_t> createByteVector(std::vector<uint8_t> contents, const RLP & rlp);
        std::string createHeaderKey(int blockNumber, std::string blockHash);

};