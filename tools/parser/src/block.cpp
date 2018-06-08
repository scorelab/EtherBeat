/*
 * Created by prabushitha on 5/19/18.
*/
#include "block.h"
#include <vector>
#include <string>

#include "address_recover.h"
#include "rlp.h"



void Header::print() {
    // printf("---------- Block Header : %d ----------\n", bytesVectorToInt(number));
    printf("parentHash: %s \n",  hexStr((unsigned char *)&parentHash_bytes[0], parentHash_bytes.size()).c_str());
    printf("sha3Uncles (ommersHash): %s \n",  hexStr((unsigned char *)&sha3Uncles_bytes[0], sha3Uncles_bytes.size()).c_str());
    printf("beneficiary (miner) : %s \n",  hexStr((unsigned char *)&beneficiary_bytes[0], beneficiary_bytes.size()).c_str());
    printf("stateRoot: %s \n",  hexStr((unsigned char *)&stateRoot_bytes[0], stateRoot_bytes.size()).c_str());
    printf("transactionsRoot: %s \n",  hexStr((unsigned char *)&transactionsRoot_bytes[0], transactionsRoot_bytes.size()).c_str());
    printf("receiptsRoot: %s \n",  hexStr((unsigned char *)&receiptsRoot_bytes[0], receiptsRoot_bytes.size()).c_str());
    printf("logsBloom: %s \n",  hexStr((unsigned char *)&logsBloom_bytes[0], logsBloom_bytes.size()).c_str());

    printf("difficulty: %d \n",  bytesVectorToInt(difficulty_bytes));
    printf("number: %d \n",  bytesVectorToInt(number_bytes));
    printf("gasLimit: %d \n",  bytesVectorToInt(gasLimit_bytes));
    printf("gasUsed: %d \n",  bytesVectorToInt(gasUsed_bytes));
    printf("timestamp: %d \n",  bytesVectorToInt(timestamp_bytes));

    printf("mixHash: %s \n",  hexStr((unsigned char *)&mixHash_bytes[0], mixHash_bytes.size()).c_str());
    printf("nonce: %s \n",  hexStr((unsigned char *)&nonce_bytes[0], nonce_bytes.size()).c_str());

}
void TransactionReceipt::print() {
    printf("blockNumber: %d \n", blockNumber);
    printf("blockHash: %s \n",  hexStr((unsigned char *)&blockHash_bytes[0], blockHash_bytes.size()).c_str());
    printf("transactionIndex: %d \n", transactionIndex);

    printf("status: %s \n",  hexStr((unsigned char *)&status_bytes[0], status_bytes.size()).c_str());
    printf("cumulativeGasUsed: %d \n",  bytesVectorToInt(cumulativeGasUsed_bytes));
    printf("logsBloom: %s \n",  hexStr((unsigned char *)&logsBloom_bytes[0], logsBloom_bytes.size()).c_str());
    printf("transactionHash: %s \n",  hexStr((unsigned char *)&transactionHash_bytes[0], transactionHash_bytes.size()).c_str());
    printf("contractAddress: %s \n",  hexStr((unsigned char *)&contractAddress_bytes[0], contractAddress_bytes.size()).c_str());
    printf("gasUsed: %d \n", bytesVectorToInt(gasUsed_bytes));
}
void Transaction::print() {
    printf("nonce: %d \n",  bytesVectorToInt(nonce_bytes));
    // printf("nonce: %s \n",  hexStr((unsigned char *)&nonce[0], nonce.size()).c_str());

    printf("gasPrice (too large int): %s\n", hexStr((unsigned char *)&gasPrice_bytes[0], gasPrice_bytes.size()).c_str());

    printf("gasLimit: %d \n",  bytesVectorToInt(gasLimit_bytes));
    // printf("gasLimit: %s \n",  hexStr((unsigned char *)&gasLimit[0], gasLimit.size()).c_str());

    printf("to: %s \n",  hexStr((unsigned char *)&to_bytes[0], to_bytes.size()).c_str());
    printf("value: %s \n",  hexStr((unsigned char *)&value_bytes[0], value_bytes.size()).c_str());
    printf("v: %s \n",  hexStr((unsigned char *)&v_bytes[0], v_bytes.size()).c_str());
    printf("r: %s \n",  hexStr((unsigned char *)&r_bytes[0], r_bytes.size()).c_str());
    printf("s: %s \n",  hexStr((unsigned char *)&s_bytes[0], s_bytes.size()).c_str());
    printf("init: %s \n",  hexStr((unsigned char *)&init_bytes[0], init_bytes.size()).c_str());
    printf("from: %s \n",  hexStr((unsigned char *)&from_bytes[0], from_bytes.size()).c_str());
    printf("hash: %s \n",  hexStr((unsigned char *)&hash_bytes[0], hash_bytes.size()).c_str());
}


std::vector<std::uint8_t> Transaction::recoverTxSender() {
    //tx FROM address
    int chainId = v_bytes[0];
    if(chainId > 0) {
        chainId = (v_bytes[0]-35)/2;
    }

    //RLP encode tx
    std::vector<RLPField> dataFields;

    RLPField field_nonce;
    field_nonce.bytes.insert(field_nonce.bytes.end(), nonce_bytes.begin(), nonce_bytes.end());
    dataFields.insert(dataFields.end(), field_nonce);

    RLPField field_gasPrice;
    field_gasPrice.bytes.insert(field_gasPrice.bytes.end(), gasPrice_bytes.begin(), gasPrice_bytes.end());
    dataFields.insert(dataFields.end(), field_gasPrice);

    RLPField field_gasLimit;
    field_gasLimit.bytes.insert(field_gasLimit.bytes.end(), gasLimit_bytes.begin(), gasLimit_bytes.end());
    dataFields.insert(dataFields.end(), field_gasLimit);

    RLPField field_to;
    field_to.bytes.insert(field_to.bytes.end(), to_bytes.begin(), to_bytes.end());
    dataFields.insert(dataFields.end(), field_to);

    RLPField field_value;
    field_value.bytes.insert(field_value.bytes.end(), value_bytes.begin(), value_bytes.end());
    dataFields.insert(dataFields.end(), field_value);

    RLPField field_init;
    field_init.bytes.insert(field_init.bytes.end(), init_bytes.begin(), init_bytes.end());
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



    uint8_t new_v = v_bytes[0];
    if (chainId > 0) {
            new_v -= (chainId * 2 + 8);
    }


    std::vector<uint8_t > public_key = recover(new_v, r_bytes, s_bytes, txHash);
    if (public_key.empty()) {
        return {};
    }
    std::vector<uint8_t> address = publicKeyToAddress(public_key);


    return address;
}

Block::Block(Header header):header(header) {}
void Block::print() {
    printf("---------- Block : %d ----------\n", bytesVectorToInt(header.number_bytes));
    printf("hash: %s \n",  bytesVectorToHexString(hash_bytes).c_str());
    header.print();
    printf("----Transactions------\n[\n");
    for(Transaction t : transactions){
        printf("{\n");
        t.print();
        printf("},\n");
    }
    printf("]\n----Uncles------\n");
    for(std::vector<uint8_t> ommerHash: ommerHashes_bytes){
        printf("ommer hash: %s \n",  hexStr((unsigned char *)&ommerHash[0], ommerHash.size()).c_str());
    }
}


