#include "Block.h"
#include <json/value.h>
#include "utils.h"
Block::Block() {}
void Block::ParseBlock(Json::Value block){
    Json::Value empty = new Json::Value();
    
    number = hex_to_int(block.get("number", empty).asString());
    hash = block.get("hash", empty).asString ();
    
    parentHash = block.get("parentHash", empty).asString ();
    nonce = block.get("nonce", empty).asString ();
    sha3Uncles = block.get("sha3Uncles", empty).asString ();
    logsBloom = block.get("logsBloom", empty).asString ();
    transactionsRoot = block.get("transactionsRoot", empty).asString ();
    stateRoot = block.get("stateRoot", empty).asString ();
    receiptsRoot = block.get("receiptsRoot", empty).asString ();
    miner = block.get("miner", empty).asString ();
    difficulty = hex_to_int(block.get("difficulty", empty).asString());
    totalDifficulty = hex_to_int(block.get("totalDifficulty", empty).asString());
    extraData = block.get("extraData", empty).asString ();
    size = hex_to_int(block.get("size", empty).asString());
    gasLimit = hex_to_int(block.get("gasLimit", empty).asString());
    gasUsed = hex_to_int(block.get("gasUsed", empty).asString());
    timestamp = hex_to_int(block.get("timestamp", empty).asString());
   
}
