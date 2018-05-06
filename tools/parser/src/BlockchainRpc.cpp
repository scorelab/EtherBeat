#include <iostream>
#include <jsonrpccpp/client.h>
#include <jsonrpccpp/client/connectors/httpclient.h>
#include "BlockchainRpc.h"
#include "utils.h"

using namespace jsonrpc;

BlockchainRpc::BlockchainRpc(std::string ethclienturl):ethclienturl(ethclienturl), httpclient(ethclienturl), client(httpclient){

}

Block BlockchainRpc::getBlock(int blockNumber){

    std::string hexid =  int_to_hex(blockNumber);
    Json::Value params;
    params[0] = hexid;
    params[1] = true;

    Block b;
    try {
        Json::Value block = client.CallMethod("eth_getBlockByNumber", params);
        b.ParseBlock(block);
    } catch (JsonRpcException &e) {
        cerr << e.what() << endl;
    }
    return b;
}

Block BlockchainRpc::getBlock(string hash){
    Json::Value params;
    params[0] = hash;
    params[1] = true;

    Block b;
    try {
        Json::Value block = client.CallMethod("eth_getBlockByHash", params);
        b.ParseBlock(block);
    } catch (JsonRpcException &e) {
        cerr << e.what() << endl;
    }
    return b;
}
int BlockchainRpc::size(){
    Json::Value params;
    try {
            Json::Value blockNumber = client.CallMethod("eth_blockNumber", params);
            return hex_to_int(blockNumber.asString());
    } catch (JsonRpcException &e) {
            cerr << e.what() << endl;
    }
    return 0;
}

