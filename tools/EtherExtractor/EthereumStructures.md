***********************
*** Block Structure ***
***********************
number              : QUANTITY - the block number. null when its pending block.
hash                : DATA, 32 Bytes - hash of the block. null when its pending block.
parentHash          : DATA, 32 Bytes - hash of the parent block.
nonce               : DATA, 8 Bytes - hash of the generated proof-of-work. null when its pending block.
sha3Uncles          : DATA, 32 Bytes - SHA3 of the uncles data in the block.
logsBloom           : DATA, 256 Bytes - the bloom filter for the logs of the block. null when its pending block.
transactionsRoot    : DATA, 32 Bytes - the root of the transaction trie of the block.
stateRoot           : DATA, 32 Bytes - the root of the final state trie of the block.
receiptsRoot        : DATA, 32 Bytes - the root of the receipts trie of the block.
miner               : DATA, 20 Bytes - the address of the beneficiary to whom the mining rewards were given.
difficulty          : QUANTITY - integer of the difficulty for this block.
totalDifficulty     : QUANTITY - integer of the total difficulty of the chain until this block.
extraData           : DATA - the "extra data" field of this block.
size                : QUANTITY - integer the size of this block in bytes.
gasLimit            : QUANTITY - the maximum gas allowed in this block.
gasUsed             : QUANTITY - the total used gas by all transactions in this block.
timestamp           : QUANTITY - the unix timestamp for when the block was collated.
transactions        : Array - Array of transaction objects, or 32 Bytes transaction hashes depending on the last given parameter.
uncles              : Array - Array of uncle hashes.

*****************************
*** Transaction Structure ***
*****************************
hash                : DATA, 32 Bytes - hash of the transaction.
nonce               : QUANTITY - the number of transactions made by the sender prior to this one.
blockHash           : DATA, 32 Bytes - hash of the block where this transaction was in. null when its pending.
blockNumber         : QUANTITY - block number where this transaction was in. null when its pending.
transactionIndex    : QUANTITY - integer of the transactions index position in the block. null when its pending.
from                : DATA, 20 Bytes - address of the sender.
to                  : DATA, 20 Bytes - address of the receiver. null when its a contract creation transaction.
value               : QUANTITY - value transferred in Wei.
gasPrice            : QUANTITY - gas price provided by the sender in Wei.
gas                 : QUANTITY - gas provided by the sender.
input               : DATA - the data send along with the transaction.

*************************************
*** Transaction Receipt Structure ***
*************************************
transactionHash     : DATA, 32 Bytes - hash of the transaction.
transactionIndex    : QUANTITY - integer of the transactions index position in the block.
blockHash           : DATA, 32 Bytes - hash of the block where this transaction was in.
blockNumber         : QUANTITY - block number where this transaction was in.
cumulativeGasUsed   : QUANTITY - The total amount of gas used when this transaction was executed in the block.
gasUsed             : QUANTITY - The amount of gas used by this specific transaction alone.
contractAddress     : DATA, 20 Bytes - The contract address created, if the transaction was a contract creation, otherwise null.
logs                : Array - Array of log objects, which this transaction generated.
logsBloom           : DATA, 256 Bytes - Bloom filter for light clients to quickly retrieve related logs.

root                : DATA 32 bytes of post-transaction stateroot (pre Byzantium)
    OR
status              : QUANTITY either 1 (success) or 0 (failure)