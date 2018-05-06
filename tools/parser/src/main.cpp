#include <iostream>
#include "Blockchain.h"
using namespace jsonrpc;
using namespace std;
int main(int argc, char *argv[]) {
   Blockchain blockchain("http://localhost:8545");
   
   Block block = blockchain.getBlock(10);
   
   // print block info
   cout << block.number << endl;
   cout << block.hash << endl;
   cout << block.stateRoot << endl;
   cout << block.timestamp << endl;
   cout << block.gasLimit << endl;
   
   return 0;
}
