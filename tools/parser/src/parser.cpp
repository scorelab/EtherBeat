#include "parser.h"
#include "utils.h"
#include "rlp.h"
#include <vector>
#include <iostream>

Parser::Parser(leveldb::DB *db):db(db) {}

Block Parser::getBlock(uint64_t blockNumber) {
    // We need to get the hash of the block first.
    std::string hashKey = getKeyString(blockNumber, headerPrefix, numSuffix, 1, 1);
    // create a string variable to store the hash
    std::string blockHash;
    // get the value from leveldb
    leveldb::Status shk = db->Get(leveldb::ReadOptions(), hashKey, &blockHash);

    if (shk.ok()) {
        // std::string test = hexStr((unsigned char *)&blockHash[0], blockHash.length());
        // print_bytes(blockHash);
        // printf("Block Hash : %s\n", test.c_str());
        // to get bytes array of a string => (uint8_t *)&string[0]
        std::string headerKey = createHeaderKey(blockNumber, blockHash); // getKeyString(blockNumber, headerPrefix, (uint8_t *)&blockHash[0], 1, 32);


        std::string blockHeaderData;
        leveldb::Status sbhd = db->Get(leveldb::ReadOptions(), headerKey, &blockHeaderData);

        if (sbhd.ok()){

            std::vector<uint8_t> contents;
            contents = getByteVector(blockHeaderData);

            RLP rlp{contents};
            // printf("RLP Items %d\n", rlp.numItems());

            Header h;
            h = createHeader(contents, rlp);
            Block b(h);
            b.hash = getByteVector(blockHash);
            return b;
        }



    }else {
        std::cout << "key not found" << std::endl;
    }

    throw;
}

//get header key
std::string Parser::createHeaderKey(int blockNumber, std::string blockHash) {
    return getKeyString(blockNumber, headerPrefix, (uint8_t *)&blockHash[0], 1, 32);
}

Header Parser::createHeader(std::vector<uint8_t> contents, RLP & rlp){
    Header header;

    header.parentHash = createByteVector(contents, rlp[0]);
    header.sha3Uncles = createByteVector(contents, rlp[1]);
    header.beneficiary = createByteVector(contents, rlp[2]);
    header.stateRoot = createByteVector(contents, rlp[3]);
    header.transactionsRoot = createByteVector(contents, rlp[4]);
    header.receiptsRoot = createByteVector(contents, rlp[5]);
    header.logsBloom = createByteVector(contents, rlp[6]);
    header.difficulty = createByteVector(contents, rlp[7]);
    header.number = createByteVector(contents, rlp[8]);
    header.gasLimit = createByteVector(contents, rlp[9]);
    header.gasUsed = createByteVector(contents, rlp[10]);
    header.timestamp = createByteVector(contents, rlp[11]);
    header.extraData = createByteVector(contents, rlp[12]);
    header.mixHash = createByteVector(contents, rlp[13]);
    header.nonce = createByteVector(contents, rlp[14]);

	return header;
}
std::vector<uint8_t> Parser::createByteVector(std::vector<uint8_t> contents, const RLP & rlp) {
    std::vector<uint8_t>::const_iterator first = contents.begin() + rlp.dataOffset();
    std::vector<uint8_t>::const_iterator last = contents.begin() + rlp.dataOffset() + rlp.dataLength();
    std::vector<uint8_t> newVec(first, last);
    return newVec;
}

