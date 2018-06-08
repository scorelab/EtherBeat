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
	    void print();
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
        std::vector<std::uint8_t> blockHash;
        uint64_t transactionIndex;

        // from db
        std::vector<std::uint8_t> status;
        std::vector<std::uint8_t> cumulativeGasUsed;
        std::vector<std::uint8_t> logsBloom;
        std::vector<std::uint8_t> transactionHash;
        std::vector<std::uint8_t> contractAddress; // if a contract creation tx, address of contract created otherwise empty
        // std::vector<std::uint8_t> logs; // array, won't consider decoding in this phase
        std::vector<std::uint8_t> gasUsed;

        void print();
};

//Transaction
class Transaction {
    public:
        std::vector<std::uint8_t> raw_tx;
        std::vector<std::uint8_t> nonce;
    	std::vector<std::uint8_t> gasPrice;
    	std::vector<std::uint8_t> gasLimit;
    	std::vector<std::uint8_t> to; // No TO address is getting for contact creation transactions
    	std::vector<std::uint8_t> value; // in wei (1 ETH = 10^18 wei)
    	std::vector<std::uint8_t> v;
    	std::vector<std::uint8_t> r;
    	std::vector<std::uint8_t> s;
    	std::vector<std::uint8_t> init; // data

    	std::vector<std::uint8_t> from;
    	std::vector<std::uint8_t> hash;

    	void print();
    	std::vector<std::uint8_t> recoverTxSender();
};



// Block
class Block {
    public:
        Header header;
        std::vector<uint8_t> hash;
        std::vector<Transaction> transactions;
        std::vector<std::vector<uint8_t>> ommerHashes;
        Block(Header header);
        void print();

};

