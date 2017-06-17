import * as graphql from "graphql";
import * as BigNumber from "bignumber.js";
import * as Web3 from "web3";
import * as contracts from "../models/contract";

let web3: Web3;
export default web3;

if (web3 !== undefined) {
  web3 = new Web3(Web3.currentProvider);
} else {
  // TODO - move to configuration
  web3 = new Web3(new Web3.providers.HttpProvider("http://localhost:8545"));
}

// web3._extend({
//   property: 'eth',
//   methods: [new web3._extend.Method({
//     name: 'trace_block',
//     call: 'trace_block',
//     params: 1,
//     inputFormatter: 0
//   })]
// });

// ress = web3.trace_block("0x2ed119");


// The root provides a resolver function for each API endpoint
export let root = {
  hello: () => {
    return "Hello world!";
  },

  contract: ({ accountHash }: { accountHash: string }) => {
    return new contracts.Contract(accountHash);
    // const balance = web3.eth.getBalance(accountHash);
    // return balance.toString(10);
  },

  contractQ: ({ accountHash }: { accountHash: string }) => {
    const balance = web3.eth.getBalance(accountHash);
    return balance.toString(10);
  },

  transaction: (id: string) => {
    return "not implemented";
  },
};