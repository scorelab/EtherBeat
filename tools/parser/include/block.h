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

// Block
class Block {
    Header header;
    public:
        std::vector<uint8_t> hash;
        Block(Header header);
        void print();

};

