import * as graphql from "graphql";

// Construct a schema, using GraphQL schema language
export let schema = graphql.buildSchema(`
type Query {
    hello: String
    contract (accountHash: String!): String
    transaction (accountHash: String!): String
}

type Contract {
    id: String
    balance: String
    storage: String
    code: String
    abiInterface: String
    transactions: [Transaction]
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