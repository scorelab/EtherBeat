/*
 * Created by prabushitha on 5/19/18.
*/
#include "block.h"
#include "utils.h"
#include "address_recover.h"
#include "rlp.h"
#include <vector>
#include <string>
#include <sstream>

/*
 * Header
 */
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
std::string Header::getParentHash(){return bytesVectorToHexString(parentHash_bytes);};
std::string Header::getSha3Uncles(){return bytesVectorToHexString(sha3Uncles_bytes);};
std::string Header::getBeneficiary(){return bytesVectorToHexString(beneficiary_bytes);};
std::string Header::getStateRoot(){return bytesVectorToHexString(stateRoot_bytes);};
std::string Header::getTransactionsRoot(){return bytesVectorToHexString(transactionsRoot_bytes);};
std::string Header::getReceiptsRoot(){return bytesVectorToHexString(receiptsRoot_bytes);};
std::string Header::getLogsBloom(){return bytesVectorToHexString(logsBloom_bytes);};
size_t Header::getDifficulty(){return bytesVectorToInt(difficulty_bytes);};
size_t Header::getNumber(){return bytesVectorToInt(number_bytes);};
size_t Header::getGasLimit(){return bytesVectorToInt(gasLimit_bytes);};
size_t Header::getGasUsed(){return bytesVectorToInt(gasUsed_bytes);};
unsigned long Header::getTimestamp(){return bytesVectorToInt(timestamp_bytes);};
std::string Header::getExtraData(){return bytesVectorToHexString(extraData_bytes);};
std::string Header::getMixHash(){return bytesVectorToHexString(mixHash_bytes);};
std::string Header::getNonce(){return bytesVectorToHexString(nonce_bytes);};

/*
 * Transaction Receipt
 */
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
size_t TransactionReceipt::getBlockNumber(){return blockNumber;};
size_t TransactionReceipt::getTransactionIndex(){ return transactionIndex;};
std::string TransactionReceipt::getBlockHash(){return bytesVectorToHexString(blockHash_bytes);};
std::string TransactionReceipt::getStatus(){return bytesVectorToHexString(status_bytes);};
size_t TransactionReceipt::getCumulativeGasUsed(){return bytesVectorToInt(cumulativeGasUsed_bytes);};
std::string TransactionReceipt::getLogsBloom(){return bytesVectorToHexString(logsBloom_bytes);};
std::string TransactionReceipt::getTransactionHash(){return bytesVectorToHexString(transactionHash_bytes);};
std::string TransactionReceipt::getContractAddress(){return bytesVectorToHexString(contractAddress_bytes);};
size_t TransactionReceipt::getGasUsed(){return bytesVectorToInt(gasUsed_bytes);};

/*
 * Transaction
 */
void Transaction::print() {
    printf("nonce: %d \n",  (int)getNonce());
    // printf("nonce: %s \n",  hexStr((unsigned char *)&nonce[0], nonce.size()).c_str());

    printf("gasPrice : %f\n", getGasPrice());

    printf("gasLimit: %d \n",  getGasLimit());
    // printf("gasLimit: %s \n",  hexStr((unsigned char *)&gasLimit[0], gasLimit.size()).c_str());

    printf("to: %s \n",  getTo().c_str());
    printf("value: %f \n",  getValue());
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
int Transaction::getNonce(){return (int)bytesVectorToInt(nonce_bytes);};
double Transaction::getGasPrice(){return hexastr2double("0x"+bytesVectorToHexString(gasPrice_bytes)); };
int Transaction::getGasLimit(){return (int)bytesVectorToInt(gasLimit_bytes);};
std::string Transaction::getTo(){return bytesVectorToHexString(to_bytes);};
double Transaction::getValue(){return hexastr2double("0x"+bytesVectorToHexString(value_bytes)); };
std::string Transaction::getV(){return bytesVectorToHexString(v_bytes);};
std::string Transaction::getR(){return bytesVectorToHexString(r_bytes);};
std::string Transaction::getS(){return bytesVectorToHexString(s_bytes);};
std::string Transaction::getData(){return bytesVectorToHexString(init_bytes);};
std::string Transaction::getFrom(){return bytesVectorToHexString(from_bytes);};
std::string Transaction::getHash(){return bytesVectorToHexString(hash_bytes);};

// value in Gwei => 1 Gwei = 10^9 wei
// unsigned long getValue(){return bytesVectorToLong(value_bytes);};
// gas price in Gwei => 1 Gwei = 10^9 wei
// unsigned long getGasPrice(){ return bytesVectorToLong(gasPrice_bytes);};

std::string Transaction::getType(){
    if (to_bytes.empty() && !init_bytes.empty()){
        return "contract creation";
    } else if(!init_bytes.empty()){
        return "message call"; // todo : this returns external--->contract txes as message calls. is it okay?
    }else{
        return "transaction call";
    }

}

/*
 * Block
 */
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
std::vector<std::string> Block::getOmmers(){
    std::vector<std::string> ommers;
    int i;
    for(i=0; i<ommerHashes_bytes.size(); i++) {
        ommers.insert(ommers.end(), bytesVectorToHexString(ommerHashes_bytes[i]));
    }
    return ommers;
}
std::vector<Transaction> Block::getTransactions(){
    return transactions;
}
std::string Block::getHash(){
    return bytesVectorToHexString(hash_bytes);
}
