import * as graphql from "graphql";

// Construct a schema, using GraphQL schema language
export let schema = graphql.buildSchema(`
type Query {
    hello: String
    contractQ (accountHash: String!): String
    contract (accountHash: String!, abiInterface: String): Contract
    transaction (accountHash: String!): String
}

type Contract {
    id: String
    balance: String
    storage: String
    address: String
    code: String
    source: String
    abiInterface: String
    creator: String
    transactions: String
}

type Transaction {
    hash: String
    nonce: Float
    blockHash: String
    blockNumber: Int
    transactionIndex: Int
    from: String
    to: String
    value: String
    gasPrice: Float
    gas: Int
    input: String
    data: String
}
`);