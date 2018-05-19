#include "block.h"
#include "rlp.h"
#include "leveldb/db.h"
#include <vector>

class Parser {

    leveldb::DB * db;

    public:
        Parser(leveldb::DB *);
        Block getBlock(uint64_t);
    private:
        Header createHeader(std::vector<uint8_t> content, RLP & rlp);
        std::vector<uint8_t> createByteVector(std::vector<uint8_t> contents, const RLP & rlp);

};