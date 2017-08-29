import * as graphql from "graphql";
import * as BigNumber from "bignumber.js";
import * as Web3 from "web3";
import * as contracts from "../models/contract";
import web3 from "../config/web3conn";

// The root provides a resolver function for each API endpoint
export let root = {
  account: ({ accountHash }: { accountHash: string }) => {
    return new contracts.Account(accountHash);
  },

  transaction: ({ hash }: { hash: string }) => {
    return new contracts.Transaction(hash);
  },
};