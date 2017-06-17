export = Web3;

import * as BigNumber from 'bignumber.js';
declare class Web3 {
    constructor(provider: Web3.Provider);

    static providers: typeof Web3.providerTypes;

    static currentProvider: Web3.Provider | null;

    currentProvider: {
        sendAsync: any;
        send: any;
    }

    // Temporary
    _extend : any;

    version: {
        api: string;
        node: string;
        network: number;
        ethereum: string;
        whisper: number;
        getNode(callback: (error: any, result: string) => any): void;
        getNetwork(callback: (error: any, result: number) => any): void;
        getWhisper(callback: (error: any, result: number) => any): void;
    };

    isConnected(): boolean;
    setProvider(provider: Web3.Provider): void;
    reset(keepIsSyncing: boolean): void;
    sha3(string: string, options?: { encoding: string }): string
    toHex(mixed: string | number | object | BigNumber.BigNumber): string;
    toAscii(hexString: string): string;
    fromAscii(string: string, padding?: number): string;
    toDecimal(hexString: string): number;
    fromDecimal(number: number | string): string;
    fromWei(number: number | string | BigNumber.BigNumber, unit: string): string | BigNumber.BigNumber;
    toWei(number: number | string | BigNumber.BigNumber, unit: string): string | BigNumber.BigNumber;
    toBigNumber(numberOrHexString: number | string): BigNumber.BigNumber;
    isAddress(address: string): boolean;

    net: {
        listening: boolean;
        peerCount: number;
        getListening(callback: (error: any, result: boolean) => any): void;
        getpeerCount(callback: (error: any, result: number) => any): void;
    }
    eth: {
        defaultAccount: string;
        defaultBlock: number | string;
        syncing: object | boolean;
        coinbase: string;
        mining: boolean;
        hashrate: number;
        gasPrice: BigNumber.BigNumber;
        accounts: string[];
        blockNumber: number;
        getCoinbase(callback: (error: any, result: string) => any): void;
        getMining(callback: (error: any, result: boolean) => any): void;
        getHashrate(callback: (error: any, result: number) => any): void;
        isSyncing(callback: (error: any, sync: any) => any): any;
        getGasPrice(callback: (error: any, result: BigNumber.BigNumber) => any): void;
        getAccounts(callback: (error: any, result: string[]) => any): void;
        getBlockNumber(callback: (error: any, result: number) => any): void;
        register(addressHexString: string, callback?: () => any): any;
        unRegister(addressHexString: string, callback?: () => any): any;
        getBalance(addressHexString: string, defaultBlock?: number | string, callback?: (error: any, result: BigNumber.BigNumber) => void): BigNumber.BigNumber;
        getStorageAt(addressHexString: string, position: number, defaultBlock?: number | string, callback?: (error: any, result: string) => void): string;
        getCode(addressHexString: string, defaultBlock?: number | string, callback?: (error: any, result: string) => void): string;
        getBlock(blockHashOrNumber: string | number, returnTransactionObjects?: boolean, callback?: (error: any, result: Web3.BlockObject) => void): Web3.BlockObject;
        getBlockTransactionCount(hashStringOrBlockNumber: string | number, callback?: (error: any, result: number) => void): number;
        getUncle(blockHashStringOrNumber: string | number, uncleNumber: number, returnTransactionObjects?: boolean, callback?: (error: any, result: object) => void): object;
        getTransaction(transactionHash: string, callback?: (error: any, result: Web3.TransactionObject) => void): Web3.TransactionObject;
        getBlockTransactionCount(hashStringOrBlockNumber: string | number, callback?: (error: any, result: number) => void): number;
        getTransactionFromBlock(hashStringOrNumber: string | number, indexNumber: number, callback?: (error: any, result: Web3.TransactionObject) => void): Web3.TransactionObject;
        getTransactionReceipt(hashString: string, callback?: (error: any, result: Web3.TransactionObject | null) => void): Web3.TransactionObject | null;
        getTransactionCount(addressHexString: string, defaultBlock?: number | string, callback?: (error: any, result: number) => void): number;
        sendTransaction(transactionObject: Web3.TransactionObject, callback?: (err: any, result: string) => void): string;
        sendRawTransaction(signedTransactionData: string, callback?: (err: any, result: string) => void): string;
        sign(address: string, dataToSign: string, callback?: (err: any, result: string) => void): string;
        call(callObject: Web3.TransactionObject, defaultBlock?: number | string, callback?: (error: any, result: string) => void): string;
        estimateGas(callObject: Web3.TransactionObject, callback?: (error: any, result: number) => void): number;
        filter(value: string | Web3.FilterObject, callback?: (error: any, result: any) => void): Web3.FilterResult;
        contract(abi: Web3.AbiDefinition[]): Web3.Contract;


        compile: {
            solidity(sourceString: string, cb?: (err: any, result: any) => void): object,
        }
        getAccounts(callback: (err: Error, value: any) => void): string[];
    };
}

/*~ If you want to expose types from your module as well, you can
 *~ place them in this block.
 */
declare namespace Web3 {
    export interface BlockObject {
        number: number | null;
        hash: string | null;
        parentHash: string;
        nonce: string | null;
        sha3Uncles: string;
        logsBloom: string | null;
        transactionsRoot: string;
        stateRoot: string;
        miner: string;
        difficulty: BigNumber.BigNumber;
        totalDifficulty: BigNumber.BigNumber;
        extraData: string;
        size: number;
        gasLimit: number;
        gasUsed: number;
        timestamp: number;
        transactions: TransactionObject[];
        uncles: string[];
    }

    export interface TransactionObject {
        hash?: string;
        nonce?: number;
        blockHash?: string | null;
        blockNumber?: number | null;
        transactionIndex?: number | null;
        from?: string;
        to?: string;
        value?: BigNumber.BigNumber;
        gasPrice?: BigNumber.BigNumber;
        gas?: number;
        input?: string;
        data?: string;
    }

    export interface AbiIOParameter {
        name: string;
        type: string;
    }

    export interface AbiDefinition {
        constant: boolean;
        inputs: AbiIOParameter[];
        name: string;
        outputs: AbiIOParameter[];
        type: string;
    }

    export interface Contract {
        at(address: string): any;
     }

    export interface FilterObject {
        fromBlock: number | string;
        toBlock: number | string;
        address: string;
        topics: string[];
    }

    export interface FilterResult {
        get(callback: () => void): void;
        watch(callback: () => void): void;
        stopWatching(): void;
    }

    export interface Provider { }

    namespace providerTypes {
        class HttpProvider implements Provider {
            constructor(url?: string);
        }
    }
}
