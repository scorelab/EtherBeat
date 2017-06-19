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

// The root provides a resolver function for each API endpoint
export let root = {
  account: ({ accountHash }: { accountHash: string }) => {
    return new contracts.Account(accountHash);
  },

  transaction: ({ hash }: { hash: string }) => {
    return new contracts.Transaction(hash);
  },
};