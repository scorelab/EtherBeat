#include <string>
#include <json/value.h>
class Block {
    public:
        Block();
        void ParseBlock(Json::Value);
        int number;
        std::string hash;
        std::string parentHash;
        std::string nonce;
        std::string sha3Uncles;
        std::string logsBloom;
        std::string transactionsRoot;
        std::string stateRoot;
        std::string receiptsRoot;
        std::string miner;
        int difficulty;
        int totalDifficulty;
        std::string extraData;
        int size;
        int gasLimit;
        int gasUsed;
        int timestamp;
};