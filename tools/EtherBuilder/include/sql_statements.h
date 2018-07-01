/*
 * Created by prabushitha on 7/1/18.
*/

// INSERTION SQL STRINGS
char const *sql_block = "INSERT INTO block (id, hash, parentHash, sha3Uncles, beneficiary, stateRoot, transactionsRoot, receiptsRoot, logsBloom, difficulty, gasLimit, gasUsed, timestamp, extraData, mixHash, nonce ) " \
                             "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, datetime( ?, 'unixepoch'), ?, ?, ? );";

char const *sql_tx  = "INSERT INTO tx (id, tx_type, nonce, gasPrice, gasLimit, receiver, value, v_val, r_val, s_val, init, sender, hash ) "  \
                           "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ? );";

char const *sql_blocktx = "INSERT INTO blocktx (blockId, txId) "  \
         "VALUES (?, ?); " ;

char const *sql_txreceipt = "INSERT INTO txreceipt (id , txHash, blockHash, blockNumber, transactionIndex, status, gasUsed, cumulativeGasUsed, contractAddress, logsBloom) "  \
         "VALUES (? , ?, ?, ?, ?, ?, ?, ?, ?, ?); " ;

char const *sql_fromto = "INSERT INTO fromto (sender , receiver, amount, txid) "  \
         "VALUES (?, ?, ?, ?); " ;
