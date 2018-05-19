#include "parser.h"
#include "utils.h"
#include "rlp.h"
#include <vector>
#include <iostream>

Parser::Parser(leveldb::DB *db):db(db) {}

Block Parser::getBlock(uint64_t blockNumber) {
    // uint64_t blockNumber = 40;
    uint8_t headerPrefix[1] = {104};
    uint8_t numSuffix[1] = {110};

    std::string hashKey = getKeyString(blockNumber, headerPrefix, numSuffix, 1, 1);
    // create a string variable to store the retrieving value from leveldb
    std::string value;
    // get the value from leveldb
    leveldb::Status shk = db->Get(leveldb::ReadOptions(), hashKey, &value);

    print_bytes(hashKey);
    if (shk.ok()) {
        std::cout << "-------- Hash Key Found --------" <<std::endl;
        print_bytes(value);
        printf("Hex (%d): %s \n", value.length(), hexStr((unsigned char *)&value[0], value.length()).c_str());

        // to get bytes array of a string => (uint8_t *)&string[0]
        std::string headerKey = getKeyString(blockNumber, headerPrefix, (uint8_t *)&value[0], 1, 32);
        print_bytes(headerKey);


        std::string blockHeaderData;
        leveldb::Status sbhd = db->Get(leveldb::ReadOptions(), headerKey, &blockHeaderData);

        if (sbhd.ok()){
            std::cout << "-------- Block Header Data Found --------" << std::endl;
            print_bytes(blockHeaderData);
            printf("Hex (%d): %s \n", blockHeaderData.length(), hexStr((unsigned char *)&blockHeaderData[0], blockHeaderData.length()).c_str());

            std::vector<uint8_t> _contents;
            int i;
            for(i=0; i<blockHeaderData.length(); i++) {
                // printf("%d ", (uint8_t)blockHeaderData[i]);
                _contents.insert(_contents.end(), (uint8_t)blockHeaderData[i]);
                // _contents.insert(_contents.end(), (uint8_t)249);
            }

            RLP rlp{_contents};
            printf("(%d) RLP Items %d\n", i, rlp.numItems());

            printf("Item : %d, size : %d\n", rlp[0].dataOffset(), rlp[0].dataLength());
            printf("Item : %d, size : %d\n", rlp[1].dataOffset(), rlp[1].dataLength());
            printf("Item : %d, size : %d\n", rlp[2].dataOffset(), rlp[2].dataLength());
            printf("Item : %d, size : %d\n", rlp[3].dataOffset(), rlp[3].dataLength());
            printf("Item : %d, size : %d\n", rlp[4].dataOffset(), rlp[4].dataLength());
            printf("Item : %d, size : %d\n", rlp[5].dataOffset(), rlp[5].dataLength());
            printf("Item : %d, size : %d\n", rlp[6].dataOffset(), rlp[6].dataLength());
            printf("Item : %d, size : %d\n", rlp[7].dataOffset(), rlp[7].dataLength());
            printf("Item : %d, size : %d\n", rlp[8].dataOffset(), rlp[8].dataLength());
            printf("Item : %d, size : %d\n", rlp[9].dataOffset(), rlp[9].dataLength());
            printf("Item : %d, size : %d\n", rlp[10].dataOffset(), rlp[10].dataLength());
            printf("Item : %d, size : %d\n", rlp[11].dataOffset(), rlp[11].dataLength());
            printf("Item : %d, size : %d\n", rlp[12].dataOffset(), rlp[12].dataLength());
            printf("Item : %d, size : %d\n", rlp[13].dataOffset(), rlp[13].dataLength());
            printf("Item : %d, size : %d\n", rlp[14].dataOffset(), rlp[14].dataLength());
        }

    }else {
        std::cout << "key not found" << std::endl;
    }
    // get the block from leveldb
    Block b;
    return b;
}