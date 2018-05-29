#include <vector>
#include <iostream>
// Block Header
class Header {
    public:
        std::vector<uint8_t> parentHash;
        std::vector<uint8_t> sha3Uncles;
        std::vector<uint8_t> beneficiary;
        std::vector<uint8_t> stateRoot;
        std::vector<uint8_t> transactionsRoot;
        std::vector<uint8_t> receiptsRoot;
        std::vector<uint8_t> logsBloom;
        std::vector<uint8_t> difficulty; //
        std::vector<uint8_t> number;
        std::vector<uint8_t> gasLimit;
        std::vector<uint8_t> gasUsed;
        std::vector<uint8_t> timestamp; //
        std::vector<uint8_t> extraData;
        std::vector<uint8_t> mixHash;
        std::vector<uint8_t> nonce;
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

