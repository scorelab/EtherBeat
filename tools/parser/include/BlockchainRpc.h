#include <jsonrpccpp/client.h>
#include <jsonrpccpp/client/connectors/httpclient.h>
#include "Block.h"

using namespace jsonrpc;
using namespace std;

class BlockchainRpc {
    public:
        BlockchainRpc(std::string);
        Block getBlock(int);
        Block getBlock(string);
        int size();
    private:
        std::string ethclienturl;
        HttpClient httpclient;
        Client client;
};