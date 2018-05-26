#include "parser.h"
#include "utils.h"
#include "rlp.h"
#include <vector>
#include <iostream>

// todo : Resize vectors (header attributes, block attributes, transaction attributes) to the required amount of memory

Parser::Parser(leveldb::DB *db):db(db) {}

Block Parser::getBlock(uint64_t blockNumber) {
    // We need to get the hash of the block first.
    std::string hashKey = getKeyString(blockNumber, headerPrefix, numSuffix, 1, 1);
    // create a string variable to store the hash
    std::string blockHash;
    // get the value from leveldb
    leveldb::Status shk = db->Get(leveldb::ReadOptions(), hashKey, &blockHash);

    if (shk.ok()) {

        std::string headerKey = createBlockHeaderKey(blockNumber, blockHash);

        std::string blockHeaderData;
        leveldb::Status sbhd = db->Get(leveldb::ReadOptions(), headerKey, &blockHeaderData);

        if (sbhd.ok()){
            // Header content retrieval success
            std::vector<uint8_t> header_content;
            header_content = getByteVector(blockHeaderData);

            RLP rlp_header{header_content};
            // printf("RLP Items %d\n", rlp_header.numItems());

            Header h;
            updateHeader(&h, header_content, rlp_header);

            // create a new block and add the header
            Block block(h);
            block.hash = getByteVector(blockHash);

            // Get block body content
            std::string bodyKey = createBlockBodyKey(blockNumber, blockHash);

            std::string blockBodyData;
            leveldb::Status sbbd = db->Get(leveldb::ReadOptions(), bodyKey, &blockBodyData);

            if (sbbd.ok()) {
                // print_bytes(blockBodyData);
                // Header content retrieval success
                std::vector<uint8_t> body_content;
                body_content = getByteVector(blockBodyData);

                RLP rlp_body{body_content};
                updateBody(&block, body_content, rlp_body);

            }else{
                printf("\n Body Not Found \n");
            }


            return block;
        }



    }else {
        std::cout << "key not found" << std::endl;
    }

    throw;
}

//get block header key
std::string Parser::createBlockHeaderKey(int blockNumber, std::string blockHash) {
    return getKeyString(blockNumber, headerPrefix, (uint8_t *)&blockHash[0], 1, 32);
}

//get block body key
std::string Parser::createBlockBodyKey(int blockNumber, std::string blockHash) {
    return getKeyString(blockNumber, bodyPrefix, (uint8_t *)&blockHash[0], 1, 32);
}

void Parser::updateHeader(Header *header, std::vector<uint8_t> contents, RLP & rlp){

    header->parentHash = createByteVector(contents, rlp[0]);
    header->sha3Uncles = createByteVector(contents, rlp[1]);
    header->beneficiary = createByteVector(contents, rlp[2]);
    header->stateRoot = createByteVector(contents, rlp[3]);
    header->transactionsRoot = createByteVector(contents, rlp[4]);
    header->receiptsRoot = createByteVector(contents, rlp[5]);
    header->logsBloom = createByteVector(contents, rlp[6]);
    header->difficulty = createByteVector(contents, rlp[7]);
    header->number = createByteVector(contents, rlp[8]);
    header->gasLimit = createByteVector(contents, rlp[9]);
    header->gasUsed = createByteVector(contents, rlp[10]);
    header->timestamp = createByteVector(contents, rlp[11]);
    header->extraData = createByteVector(contents, rlp[12]);
    header->mixHash = createByteVector(contents, rlp[13]);
    header->nonce = createByteVector(contents, rlp[14]);

}

void Parser::updateBody(Block *block, std::vector<uint8_t> contents, RLP & rlp){
    if (rlp.numItems() > 1) {
        int i;
        // get transactions
        for(i=0; i<rlp[0].numItems(); i++) {
            // todo : check if transaction contains valid number of fields
            Transaction transaction;
            transaction.raw_tx = createByteVector(contents, rlp[0][i]);
            transaction.nonce = createByteVector(contents, rlp[0][i][0]);
            transaction.gasPrice = createByteVector(contents, rlp[0][i][1]);
            transaction.gasLimit = createByteVector(contents, rlp[0][i][2]);
            transaction.to = createByteVector(contents, rlp[0][i][3]); // empty for contract creation
            transaction.value = createByteVector(contents, rlp[0][i][4]);
            transaction.init = createByteVector(contents, rlp[0][i][5]);

            transaction.v = createByteVector(contents, rlp[0][i][6]);
            transaction.r = createByteVector(contents, rlp[0][i][7]);
            transaction.s = createByteVector(contents, rlp[0][i][8]);

            transaction.from = transaction.recoverTxSender();

            // todo : create from field using txHash, v, r, s values
            block->transactions.insert(block->transactions.end(), transaction);

        }

        // todo : check below code after a full node.
        // get ommers
        std::vector<std::vector<uint8_t>> ommerHashes;

        for(i=0; i<rlp[1].numItems(); i++) {
            std::vector<uint8_t> ommerHash = createByteVector(contents, rlp[1][i]);
            ommerHashes.insert(ommerHashes.end(), ommerHash);
        }
        block->ommerHashes = ommerHashes;
    }

}
std::vector<uint8_t> Parser::createByteVector(std::vector<uint8_t> contents, const RLP & rlp) {
    std::vector<uint8_t>::const_iterator first = contents.begin() + rlp.dataOffset();
    std::vector<uint8_t>::const_iterator last = contents.begin() + rlp.dataOffset() + rlp.dataLength();
    std::vector<uint8_t> newVec(first, last);
    return newVec;
}

