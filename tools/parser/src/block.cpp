#include "block.h"
#include "utils.h"
#include <vector>
#include <string>

#include "address_recover.h"
#include "rlp.h"



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
    // printf("nonce: %s \n",  hexStr((unsigned char *)&nonce[0], nonce.size()).c_str());

    printf("gasPrice (too large int): %s\n", hexStr((unsigned char *)&gasPrice[0], gasPrice.size()).c_str());

    printf("gasLimit: %d \n",  bytesVectorToInt(gasLimit));
    // printf("gasLimit: %s \n",  hexStr((unsigned char *)&gasLimit[0], gasLimit.size()).c_str());

    printf("to: %s \n",  hexStr((unsigned char *)&to[0], to.size()).c_str());
    printf("value: %s \n",  hexStr((unsigned char *)&value[0], value.size()).c_str());
    printf("v: %s \n",  hexStr((unsigned char *)&v[0], v.size()).c_str());
    printf("r: %s \n",  hexStr((unsigned char *)&r[0], r.size()).c_str());
    printf("s: %s \n",  hexStr((unsigned char *)&s[0], s.size()).c_str());
    printf("init: %s \n",  hexStr((unsigned char *)&init[0], init.size()).c_str());
    printf("from: %s \n",  hexStr((unsigned char *)&from[0], from.size()).c_str());
    printf("hash: %s \n",  hexStr((unsigned char *)&hash[0], hash.size()).c_str());
}


std::vector<std::uint8_t> Transaction::recoverTxSender() {
    //tx FROM address
    int chainId = v[0];
    if(chainId > 0) {
        chainId = (v[0]-35)/2;
    }

    //RLP encode tx
    std::vector<RLPField> dataFields;

    RLPField field_nonce;
    field_nonce.bytes.insert(field_nonce.bytes.end(), nonce.begin(), nonce.end());
    dataFields.insert(dataFields.end(), field_nonce);

    RLPField field_gasPrice;
    field_gasPrice.bytes.insert(field_gasPrice.bytes.end(), gasPrice.begin(), gasPrice.end());
    dataFields.insert(dataFields.end(), field_gasPrice);

    RLPField field_gasLimit;
    field_gasLimit.bytes.insert(field_gasLimit.bytes.end(), gasLimit.begin(), gasLimit.end());
    dataFields.insert(dataFields.end(), field_gasLimit);

    RLPField field_to;
    field_to.bytes.insert(field_to.bytes.end(), to.begin(), to.end());
    dataFields.insert(dataFields.end(), field_to);

    RLPField field_value;
    field_value.bytes.insert(field_value.bytes.end(), value.begin(), value.end());
    dataFields.insert(dataFields.end(), field_value);

    RLPField field_init;
    field_init.bytes.insert(field_init.bytes.end(), init.begin(), init.end());
    dataFields.insert(dataFields.end(), field_init);

    if (chainId > 0) {
        RLPField field_v;
        field_v.bytes.insert(field_v.bytes.end(), chainId);
        dataFields.insert(dataFields.end(), field_v);

        RLPField field_s;
        dataFields.insert(dataFields.end(), field_s);

        RLPField field_r;
        dataFields.insert(dataFields.end(), field_r);
    }


    std::vector<uint8_t> encoded_tx;
    encoded_tx = RLP::serialize(dataFields);

    // printf("RLP ENCODED TX : %s\n", hexStr((unsigned char *)&encoded_tx[0], encoded_tx.size()).c_str());
    std::string test1 = hexStr((unsigned char *)&encoded_tx[0], encoded_tx.size());
    std::vector<uint8_t> txHash = keccak_256(encoded_tx);
    std::string test2 = hexStr((unsigned char *)&txHash[0], txHash.size());



    uint8_t new_v = v[0];
    if (chainId > 0) {
            new_v -= (chainId * 2 + 8);
    }


    std::vector<uint8_t > public_key = recover(new_v, r, s, txHash);
    if (public_key.empty()) {
        return {};
    }
    std::vector<uint8_t> address = publicKeyToAddress(public_key);


    return address;
}

Block::Block(Header header):header(header) {}
void Block::print() {
    printf("---------- Block : %d ----------\n", bytesVectorToInt(header.number));
    printf("hash: %s \n",  hexStr((unsigned char *)&hash[0], hash.size()).c_str());
    header.print();
    printf("----Transactions------\n[\n");
    for(Transaction t : transactions){
        printf("{\n");
        t.print();
        printf("},\n");
    }
    printf("]\n----Uncles------\n");
    for(std::vector<uint8_t> ommerHash: ommerHashes){
        printf("ommer hash: %s \n",  hexStr((unsigned char *)&ommerHash[0], ommerHash.size()).c_str());
    }
}


