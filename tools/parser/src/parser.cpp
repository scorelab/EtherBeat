#include "parser.h"
#include "utils.h"
#include "rlp.h"
#include <vector>
#include <iostream>

// todo : Resize vectors (header attributes, block attributes, transaction attributes) to the required amount of memory

Parser::Parser(leveldb::DB *db):db(db) {}
/*
*
* BLOCK RELATED PARSING
*
*/
Block Parser::getBlock(uint64_t blockNumber) {
    // We need to get the hash of the block first.
    std::string hashKey = getKeyString(blockNumber, headerPrefix, numSuffix, 1, 1);
    // create a string variable to store the hash
    std::string blockHash;

    std::string test = hexStr((unsigned char *)&hashKey[0], hashKey.size()).c_str();
    leveldb::Status shk = db->Get(leveldb::ReadOptions(), hashKey, &blockHash);

    if (shk.ok()) {
        return getBlock(blockNumber, blockHash);
    }

    throw;
}

Block Parser::getBlock(std::string blockHashHex) {

    blockHashHex = remove0xFromString(blockHashHex);

    std::vector<uint8_t > hexval = hex_to_bytes(blockHashHex);

    // block hash byte string
    std::string blockHash(hexval.begin(), hexval.end());

    // append db key prefix
    hexval.insert(hexval.begin(), blockHashPrefix[0]);
    std::string keyString(hexval.begin(), hexval.end());

    // create a string variable to store the number
    std::string blockNumber;
    // get the value from leveldb
    leveldb::Status shk = db->Get(leveldb::ReadOptions(), keyString, &blockNumber);

    if (shk.ok()) {
        uint64_t number = bytesVectorToInt(getByteVector(blockNumber));
        return getBlock(number, blockHash);
    } else {
        printf("INVALID HASH : %s\n", shk.ToString().c_str());
    }

    throw;
}

Block Parser::getBlock(uint64_t blockNumber, std::string blockHash){

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

            // calculate the hash of the transaction
            std::vector<uint8_t> rlp_encoded_tx = rlp[0][i].serializedData();
            transaction.hash = keccak_256(rlp_encoded_tx);

            transaction.from = transaction.recoverTxSender();

            if(transaction.from.size() == 0) {
                printf("Block %d Transaction %d Address Not Found\n", bytesVectorToInt(block->header.number), i+1);
            }

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

/*
*
*   ACCOUNT RELATED PARSING
*
*/

Account Parser::getAccount(std::string address) {
    // todo : get the latest block number
    uint64_t latestBlock = 100;//189154;
    return getAccount(address, latestBlock);
}

Account Parser::getAccount(std::string address, uint64_t blockHeight) {
    // todo : has to implement using merkel patricia tree
    Block b = getBlock(blockHeight);

    if(!b.header.stateRoot.empty()){

        // std::vector<uint8_t > raw_key = getByteVector(address);
        // std::vector<uint8_t > hash_key = keccak_256(raw_key);
        // std::string key = hexStr((unsigned char *)&hash_key[0], hash_key.size());

        std::string value;

        // Get byte string of state root
        std::string stateRoot (b.header.stateRoot.begin(), b.header.stateRoot.end());
        leveldb::Status status = db->Get(leveldb::ReadOptions(), stateRoot, &value);

        if (status.ok()) {
            printf("ADDRESS FOUND\n");
            print_bytes(value);
        }else {
            printf("ADDRESS NOT FOUND\n");
        }
    }


    Account account;

    return account;
}