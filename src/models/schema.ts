import * as graphql from "graphql";

// Construct a schema, using GraphQL schema language
export let schema = graphql.buildSchema(`
type Query {
    account (accountHash: String!, abiInterface: String): Account
    transaction (hash: String!): Transaction
}

type Account {
    id: String
    balance: String!
    storage: String
    address: String!
    code: String
    source: String
    abiInterface: String
    creator: String
    transactions (page: Int = 1, size: Int = 10): [Transaction]
    parameters (jsonAbi: String!): String
}

type Transaction {
    hash: String
    nonce: Float
    blockHash: String
    blockNumber: Int
    transactionIndex: Int
    from: Account
    to: Account
    value: String
    gasPrice: String
    gas: Int
    input: String
    data: String
}
`);