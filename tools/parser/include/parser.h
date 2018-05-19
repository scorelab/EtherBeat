#include "block.h"
#include "leveldb/db.h"

class Parser {

    leveldb::DB * db;

    public:
        Parser(leveldb::DB *);
        Block getBlock(uint64_t);

};
