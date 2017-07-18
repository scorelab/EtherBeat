import * as Web3 from "web3";

let web3: Web3;

if (web3 !== undefined) {
    web3 = new Web3(Web3.currentProvider);
} else {
    web3 = new Web3(new Web3.providers.HttpProvider("http://localhost:8545"));
}

export default web3;