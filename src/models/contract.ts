import * as Web3 from "web3";

let web3: Web3;
export default web3;

if (web3 !== undefined) {
    web3 = new Web3(Web3.currentProvider);
} else {
    // TODO - move to configuration
    web3 = new Web3(new Web3.providers.HttpProvider("http://localhost:8545"));
}

export class Contract {
    id: string;
    address: string;
    abiInterface: string;

    constructor(address: string, abiInterface?: string) {
        this.address = address;
        this.abiInterface = abiInterface;
    }

    balance() {
        const balance = web3.eth.getBalance(this.address);
        return balance.toString(10);
    }

    storage() {
        return web3.eth.getStorageAt(this.address, 0);
    }

    code() {
        return web3.eth.getCode(this.address);
    }

    source() {
        // TODO
        return "";
    }

    creator() {
        // TODO
        return "";
    }

    transactions() {
        // TODO
        // return "";
        const sds = web3.currentProvider.send({
            method: "trace_filter",
            params: [{ fromBlock: web3.fromDecimal(1038780), toAddress: [this.address] }],
            jsonrpc: "2.0",
            id: "1"
        });
        return JSON.stringify(sds.result);
    }
}

export class Transaction {

}