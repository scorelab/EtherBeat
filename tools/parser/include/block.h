/*
 * Created by prabushitha on 5/19/18.
*/
#include <vector>
#include <iostream>
#include "utils.h"

// Block Header
class Header {
    public:
        std::vector<uint8_t> parentHash_bytes;
        std::vector<uint8_t> sha3Uncles_bytes;
        std::vector<uint8_t> beneficiary_bytes;
        std::vector<uint8_t> stateRoot_bytes;
        std::vector<uint8_t> transactionsRoot_bytes;
        std::vector<uint8_t> receiptsRoot_bytes;
        std::vector<uint8_t> logsBloom_bytes;
        std::vector<uint8_t> difficulty_bytes; //
        std::vector<uint8_t> number_bytes;
        std::vector<uint8_t> gasLimit_bytes;
        std::vector<uint8_t> gasUsed_bytes;
        std::vector<uint8_t> timestamp_bytes; //
        std::vector<uint8_t> extraData_bytes;
        std::vector<uint8_t> mixHash_bytes;
        std::vector<uint8_t> nonce_bytes;
        // methods
	    void print();
	    std::string getParentHash(){return bytesVectorToHexString(parentHash_bytes);};
	    std::string getSha3Uncles(){return bytesVectorToHexString(sha3Uncles_bytes);};
	    std::string getBeneficiary(){return bytesVectorToHexString(beneficiary_bytes);};
	    std::string getStateRoot(){return bytesVectorToHexString(stateRoot_bytes);};
	    std::string getTransactionsRoot(){return bytesVectorToHexString(transactionsRoot_bytes);};
	    std::string getReceiptsRoot(){return bytesVectorToHexString(receiptsRoot_bytes);};
	    std::string getLogsBloom(){return bytesVectorToHexString(logsBloom_bytes);};
	    size_t getDifficulty(){return bytesVectorToInt(difficulty_bytes);};
		size_t getNumber(){return bytesVectorToInt(number_bytes);};
		size_t getGasLimit(){return bytesVectorToInt(gasLimit_bytes);};
		size_t getGasUsed(){return bytesVectorToInt(gasUsed_bytes);};
		unsigned long getTimestamp(){return bytesVectorToInt(timestamp_bytes);};
	    std::string getExtraData(){return bytesVectorToHexString(extraData_bytes);};
	    std::string getMixHash(){return bytesVectorToHexString(mixHash_bytes);};
	    std::string getNonce(){return bytesVectorToHexString(nonce_bytes);};
};
// Transaction Receipt Meta
struct TransactionReceiptMeta {
    uint64_t index;
    uint64_t blockNumber;
    std::string blockHash;
    uint8_t status = 0;
};
//Transaction Receipt
class TransactionReceipt {
    public:
        uint64_t blockNumber;
		uint64_t transactionIndex;

        std::vector<std::uint8_t> blockHash_bytes;


        // from db
        std::vector<std::uint8_t> status_bytes;
        std::vector<std::uint8_t> cumulativeGasUsed_bytes;
        std::vector<std::uint8_t> logsBloom_bytes;
        std::vector<std::uint8_t> transactionHash_bytes;
        std::vector<std::uint8_t> contractAddress_bytes; // if a contract creation tx, address of contract created otherwise empty
        // std::vector<std::uint8_t> logs_bytes; // array, won't consider decoding in this phase
        std::vector<std::uint8_t> gasUsed_bytes;

        void print();
        size_t getBlockNumber(){return blockNumber;};
        size_t getTransactionIndex(){ return transactionIndex;};
		std::string getBlockHash(){return bytesVectorToHexString(blockHash_bytes);};
		std::string getStatus(){return bytesVectorToHexString(status_bytes);};
		size_t getCumulativeGasUsed(){return bytesVectorToInt(cumulativeGasUsed_bytes);};
		std::string getLogsBloom(){return bytesVectorToHexString(logsBloom_bytes);};
		std::string getTransactionHash(){return bytesVectorToHexString(transactionHash_bytes);};
		std::string getContractAddress(){return bytesVectorToHexString(contractAddress_bytes);};
		size_t getGasUsed(){return bytesVectorToInt(gasUsed_bytes);};

};

//Transaction
class Transaction {
    public:
        std::vector<std::uint8_t> nonce_bytes;
    	std::vector<std::uint8_t> gasPrice_bytes;
    	std::vector<std::uint8_t> gasLimit_bytes;
    	std::vector<std::uint8_t> to_bytes; // No TO address is getting for contact creation transactions
    	std::vector<std::uint8_t> value_bytes; // in wei (1 ETH = 10^18 wei)
    	std::vector<std::uint8_t> v_bytes;
    	std::vector<std::uint8_t> r_bytes;
    	std::vector<std::uint8_t> s_bytes;
    	std::vector<std::uint8_t> init_bytes; // data

    	std::vector<std::uint8_t> from_bytes;
    	std::vector<std::uint8_t> hash_bytes;

    	void print();
    	std::vector<std::uint8_t> recoverTxSender();
    	// getters
		int getNonce(){return (int)bytesVectorToInt(nonce_bytes);};
		std::string getGasPrice(){return bytesVectorToHexString(gasPrice_bytes);};
		int getGasLimit(){return (int)bytesVectorToInt(gasLimit_bytes);};
		std::string getTo(){return bytesVectorToHexString(to_bytes);};
		std::string getValue(){return bytesVectorToHexString(value_bytes);};
		std::string getV(){return bytesVectorToHexString(v_bytes);};
		std::string getR(){return bytesVectorToHexString(r_bytes);};
		std::string getS(){return bytesVectorToHexString(s_bytes);};
		std::string getData(){return bytesVectorToHexString(init_bytes);};
		std::string getFrom(){return bytesVectorToHexString(from_bytes);};
		std::string getHash(){return bytesVectorToHexString(hash_bytes);};

		// value in Gwei => 1 Gwei = 10^9 wei
		// unsigned long getValue(){return bytesVectorToLong(value_bytes);};
		// gas price in Gwei => 1 Gwei = 10^9 wei
		// unsigned long getGasPrice(){ return bytesVectorToLong(gasPrice_bytes);};
};



// Block
class Block {
    public:
        Header header;
        std::vector<uint8_t> hash_bytes;
        std::vector<Transaction> transactions;
        std::vector<std::vector<uint8_t>> ommerHashes_bytes;
        Block(Header header);
        void print();

        //getters
		std::string getHash(){return bytesVectorToHexString(hash_bytes);};
		std::vector<Transaction> getTransactions(){return transactions;};
		std::vector<std::string> getOmmers(){
			std::vector<std::string> ommers;
			int i;
			for(i=0; i<ommerHashes_bytes.size(); i++) {
				ommers.insert(ommers.end(), bytesVectorToHexString(ommerHashes_bytes[i]));
			}
			return ommers;
		};

};

