/*
 * Created by prabushitha on 5/19/18.
*/
#include <vector>
#include <iostream>

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
	    std::string getParentHash();
	    std::string getSha3Uncles();
	    std::string getBeneficiary();
	    std::string getStateRoot();
	    std::string getTransactionsRoot();
	    std::string getReceiptsRoot();
	    std::string getLogsBloom();
	    size_t getDifficulty();
		size_t getNumber();
		size_t getGasLimit();
		size_t getGasUsed();
		unsigned long getTimestamp();
	    std::string getExtraData();
	    std::string getMixHash();
	    std::string getNonce();
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
        size_t getBlockNumber();
        size_t getTransactionIndex();
		std::string getBlockHash();
		std::string getStatus();
		size_t getCumulativeGasUsed();
		std::string getLogsBloom();
		std::string getTransactionHash();
		std::string getContractAddress();
		size_t getGasUsed();

};

//Transaction
class Transaction {
    public:
        std::vector<std::uint8_t> nonce_bytes;
    	std::vector<std::uint8_t> gasPrice_bytes;
    	std::vector<std::uint8_t> gasLimit_bytes; // gas limit*gas price = max tx fee
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
		int getNonce();
		double getGasPrice();
		int getGasLimit();
		std::string getTo();
		double getValue();
		std::string getV();
		std::string getR();
		std::string getS();
		std::string getData();
		std::string getFrom();
		std::string getHash();
		std::string getType();
};



// Block
class Block {
    public:
        Header header;
        std::vector<uint8_t> hash_bytes;
        std::vector<Transaction> transactions;
        std::vector<std::vector<uint8_t>> ommerHashes_bytes;

        explicit Block(Header header);
        void print();

        //getters
		std::string getHash();
		std::vector<Transaction> getTransactions();
		std::vector<std::string> getOmmers();

};

