/*
 * Created by prabushitha on 5/19/18.
*/
#include "block.h"
#include <vector>
#include <string>
#include <sstream>

#include "address_recover.h"
#include "rlp.h"



void Header::print() {
    // printf("---------- Block Header : %d ----------\n", bytesVectorToInt(number));
    printf("parentHash: %s \n",  getParentHash().c_str());
    printf("sha3Uncles (ommersHash): %s \n",  getSha3Uncles().c_str());
    printf("beneficiary (miner) : %s \n",  getBeneficiary().c_str());
    printf("stateRoot: %s \n",  getStateRoot().c_str());
    printf("transactionsRoot: %s \n",  getTransactionsRoot().c_str());
    printf("receiptsRoot: %s \n",  getReceiptsRoot().c_str());
    printf("logsBloom: %s \n",  getLogsBloom().c_str());

    printf("difficulty: %d \n",  (int)getDifficulty());
    printf("number: %d \n",  (int)getNumber());
    printf("gasLimit: %d \n",  (int)getGasLimit());
    printf("gasUsed: %d \n",  (int)getGasUsed());
    printf("timestamp: %lu \n",  getTimestamp());

    printf("mixHash: %s \n",  getMixHash().c_str());
    printf("nonce: %s \n",  getNonce().c_str());

}
void TransactionReceipt::print() {
    printf("blockNumber: %d \n", (int)getBlockNumber());
    printf("blockHash: %s \n",  getBlockHash().c_str());
    printf("transactionIndex: %d \n", (int)getTransactionIndex());

    printf("status: %s \n",  getStatus().c_str());
    printf("cumulativeGasUsed: %d \n",  (int)getCumulativeGasUsed());
    printf("logsBloom: %s \n",  getLogsBloom().c_str());
    printf("transactionHash: %s \n",  getTransactionHash().c_str());
    printf("contractAddress: %s \n",  getContractAddress().c_str());
    printf("gasUsed: %d \n", (int)getGasUsed());
}
void Transaction::print() {
    printf("nonce: %d \n",  (int)getNonce());
    // printf("nonce: %s \n",  hexStr((unsigned char *)&nonce[0], nonce.size()).c_str());

    printf("gasPrice : %s\n", getGasPrice().c_str());

    printf("gasLimit: %d \n",  getGasLimit());
    // printf("gasLimit: %s \n",  hexStr((unsigned char *)&gasLimit[0], gasLimit.size()).c_str());

    printf("to: %s \n",  getTo().c_str());
    printf("value: %s \n",  getValue().c_str());
    printf("v: %s \n",  getV().c_str());
    printf("r: %s \n",  getR().c_str());
    printf("s: %s \n",  getS().c_str());
    printf("init: %s \n",  getData().c_str());
    printf("from: %s \n",  getFrom().c_str());
    printf("hash: %s \n",  getHash().c_str());
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
    printf("hash: %s \n",  getHash().c_str());
    header.print();
    printf("----Transactions------\n[\n");
    for(Transaction t : transactions){
        printf("{\n");
        t.print();
        printf("},\n");
    }
    printf("]\n----Uncles------\n");
    std::vector<std::string> ommers = getOmmers();
    for(std::string ommer: ommers){
        printf("ommer hash: %s \n",  ommer.c_str());
    }
}


