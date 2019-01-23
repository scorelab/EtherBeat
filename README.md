# EtherBeat

![Logo](/images/logo.jpg)

With the growing popularity of BitCoin in the last couple of years, other blockchains have been in development for solving various problems that need distributed consensus. [Ethereum](https://www.ethereum.org/) blockchain is one such example, that gives users a platform to develop "smart contracts" using their Turing complete language [Solidity](https://solidity.readthedocs.io/en/v0.4.25/).  This gives the ability to develop decentralized applications (or dApps). Users do not need to 'trust' anything or anybody.  In addition to that, dApps are imutable and will remain available and unchanged in future. Also, it is resistant to traditional attacks such as DDoS. 


This framework has enabled developers to write decentralized applications without much effort, without needing distributed systems or cryptography knowledge. Many startups are actively developing applications for Ethereum such as [Ethereum Name Service (ENS)](https://ens.domains/), Etheria, [WeiFund](http://weifund.io/) and [Consensys](https://new.consensys.net/).  Unfortunately, because the technology being used is bleeding edge it is inevitable that attacks or hacks will target these apps for monetary reasons.  Last year [DAO](https://medium.com/swlh/the-story-of-the-dao-its-history-and-consequences-71e6a8a551ee) suffered a severe attack because of its security flaws. Due to that attack, 3.6m ether was stolen from DAO's smart contract.  Also, once you publish a smart contract in Ethereum it is not possible to modify or update it as a regular web application. Thus, even when developers identify a critical bug in the contract it is not possible to push a bugfix in a straightforward way. 


Thus, in order to fill the gap of not having a proper 'smart contract' compatible monitoring service, we propose to build a web application that can monitor other smart contracts in Ethereum and give the capability to safeguard it's critical functions (sending and receiving Ether) and interact with and visualize the smart contract functions in a much simpler way.  Also depending smart contracts can use our base smart contracts to gain the advantage of having 'circuit-breaker' which will pause the activity if things go wrong in an unexpected way.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

Make sure you have installed,
  - [Node.js](http://nodejs.org/) (v6 and above)
  - [Apache TinkerPop server](http://tinkerpop.apache.org/) (v7.2.5)
  - Python 3

EtherBeat depends on a Web3 compatible JSON-RPC server like [Parity](https://github.com/paritytech/parity) or [Geth](https://github.com/ethereum/go-ethereum/wiki/geth) Ethereum node to query information.
Enhanced querying capability is provided by TinkerPop server that will store the blockchain in a graph database.

Please follow instructions at [here](http://tinkerpop.apache.org/docs/current/tutorials/getting-started/) to setup your TinkerPop instance.
Initial syncing process of Ethereum mainnet network with TinkerPop will take a considerable amount of time, therefore we recommend you to work with a test network to get started.
Using Ethereum extractor provided in extractor.py, user can sync the graph database instance.

After Parity/Geth and TinkerPop server is running, you can start the node.js application.
If there are any changes in default configuration, please make sure to edit them in config/web3conn.js file.

```
git clone git@github.com:scorelab/EtherBeat.git # or clone your own fork
cd EtherBeat
npm install
```

After running the node.js app, you can browse the grahql query instance at http://localhost:3000/graphql.

### Examples

### Getting account balance

```
{
  account (accountHash: "0xf7d93bcb8e4372f46383ecee82f9adf1aa397ba9") {
    balance
  }
}
```

### Getting creator address (for smart contract)

```
{
  account (accountHash: "0xf7d93bcb8e4372f46383ecee82f9adf1aa397ba9") {
    creator
  }
}
```

### Getting latest 10 transactions

```
{
  account (accountHash: "0xf7d93bcb8e4372f46383ecee82f9adf1aa397ba9") {
    transactions {
      value
    }
  }
}
```

### Getting transactions with pagination

```
{
  account(accountHash: "0xf7d93bcb8e4372f46383ecee82f9adf1aa397ba9") {
    transactions(page: 2, size: 100) {
      value
    }
  }
}
```

### Nested queries

Since `from` and `to` fields are Ethereum addresses it is possible treat them as accounts in EtherBeat, which gives the ability to do nested queries.

```
{
  account(accountHash: "0xf7d93bcb8e4372f46383ecee82f9adf1aa397ba9") {
    transactions {
      from {
        address
        balance
      }
      to {
        address
        balance
      }
    }
  }
}
```

#### Reading smart contract values

```
{
  account(accountHash: "0xf7d93bcb8e4372f46383ecee82f9adf1aa397ba9") {
    parameters (jsonAbi: "provide JSON abi interface here")
  }
}

```

Result are returned as a list of (function name, variable type, result)

```
{
  "data": {
    "account": {
      "parameters": "[{\"function\":\"newVersionReleased\",\"type\":[{\"name\":\"\",\"type\":\"bool\"}],\"result\":false},{\"function\":\"creationBlockNumber\",\"type\":[{\"name\":\"\",\"type\":\"uint256\"}],\"result\":\"639374\"},{\"function\":\"extraBalance\",\"type\":[{\"name\":\"\",\"type\":\"uint256\"}],\"result\":\"5.1707074912426014124443e+22\"},{\"function\":\"version\",\"type\":[{\"name\":\"\",\"type\":\"string\"}],\"result\":\"0.1.0\"},{\"function\":\"subsidyFactor\",\"type\":[{\"name\":\"\",\"type\":\"uint256\"}],\"result\":\"2\"}]"
    }
  }
}
```