#include "block.h"
#include "utils.h"
#include <vector>
#include <string>

void Header::print() {
    // printf("---------- Block Header : %d ----------\n", bytesVectorToInt(number));
    printf("parentHash: %s \n",  hexStr((unsigned char *)&parentHash[0], parentHash.size()).c_str());
    printf("sha3Uncles (ommersHash): %s \n",  hexStr((unsigned char *)&sha3Uncles[0], sha3Uncles.size()).c_str());
    printf("beneficiary (miner) : %s \n",  hexStr((unsigned char *)&beneficiary[0], beneficiary.size()).c_str());
    printf("stateRoot: %s \n",  hexStr((unsigned char *)&stateRoot[0], stateRoot.size()).c_str());
    printf("transactionsRoot: %s \n",  hexStr((unsigned char *)&transactionsRoot[0], transactionsRoot.size()).c_str());
    printf("receiptsRoot: %s \n",  hexStr((unsigned char *)&receiptsRoot[0], receiptsRoot.size()).c_str());
    printf("logsBloom: %s \n",  hexStr((unsigned char *)&logsBloom[0], logsBloom.size()).c_str());

    printf("difficulty: %d \n",  bytesVectorToInt(difficulty));
    printf("number: %d \n",  bytesVectorToInt(number));
    printf("gasLimit: %d \n",  bytesVectorToInt(gasLimit));
    printf("gasUsed: %d \n",  bytesVectorToInt(gasUsed));
    printf("timestamp: %d \n",  bytesVectorToInt(timestamp));

    printf("mixHash: %s \n",  hexStr((unsigned char *)&mixHash[0], mixHash.size()).c_str());
    printf("nonce: %s \n",  hexStr((unsigned char *)&nonce[0], nonce.size()).c_str());

}

void Transaction::print() {
    printf("nonce: %d \n",  bytesVectorToInt(nonce));
    printf("gasPrice (too large int): %s\n", hexStr((unsigned char *)&gasPrice[0], gasPrice.size()).c_str());
    printf("gasLimit: %d \n",  bytesVectorToInt(gasLimit));
    printf("to: %s \n",  hexStr((unsigned char *)&to[0], to.size()).c_str());
    printf("value: %s \n",  hexStr((unsigned char *)&value[0], value.size()).c_str());
    printf("v: %s \n",  hexStr((unsigned char *)&v[0], v.size()).c_str());
    printf("r: %s \n",  hexStr((unsigned char *)&r[0], r.size()).c_str());
    printf("s: %s \n",  hexStr((unsigned char *)&s[0], s.size()).c_str());
    printf("init: %s \n",  hexStr((unsigned char *)&init[0], init.size()).c_str());
    printf("\n \n");
}


Block::Block(Header header):header(header) {}
void Block::print() {
    printf("---------- Block : %d ----------\n", bytesVectorToInt(header.number));
    printf("hash: %s \n",  hexStr((unsigned char *)&hash[0], hash.size()).c_str());
    header.print();
    printf("----Transactions------\n");
    for(Transaction t : transactions){
        t.print();
    }

    for(std::vector<uint8_t> ommerHash: ommerHashes){
        printf("ommer hash: %s \n",  hexStr((unsigned char *)&ommerHash[0], ommerHash.size()).c_str());
    }
}


