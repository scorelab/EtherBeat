import * as graphql from "graphql";
import * as BigNumber from "bignumber.js";
import * as Web3 from "web3";

let web3: Web3;
if ( web3 !== undefined) {
  web3 = new Web3(Web3.currentProvider);
} else {
  // TODO - move to configuration
  web3 = new Web3(new Web3.providers.HttpProvider("http://localhost:8545"));
}


// The root provides a resolver function for each API endpoint
export let root = {
  hello: () => {
    return "Hello world!";
  },

  contract: ({accountHash}: {accountHash: string}) => {
    const balance = web3.eth.getBalance(accountHash);
    return balance.toString(10);
  },

  transaction: (id: string) => {
    return "not implemented";
  },
};