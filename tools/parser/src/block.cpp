#include "block.h"
#include "utils.h"
#include <vector>
#include <string>

void Header::print() {
    printf("---------- Block Header : %d ----------\n", bytesVectorToInt(number));
    printf("parentHash: %s \n",  hexStr((unsigned char *)&parentHash[0], parentHash.size()).c_str());
    printf("sha3Uncles: %s \n",  hexStr((unsigned char *)&sha3Uncles[0], sha3Uncles.size()).c_str());
    printf("beneficiary: %s \n",  hexStr((unsigned char *)&beneficiary[0], beneficiary.size()).c_str());
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
Block::Block(Header header):header(header) {}