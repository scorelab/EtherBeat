import time
import numpy as np

from threading import Thread
from gremlin_python.driver.driver_remote_connection import DriverRemoteConnection
from gremlin_python.structure.graph import Graph
from web3 import Web3, KeepAliveRPCProvider

web3 = Web3(KeepAliveRPCProvider(host='localhost', port='8545'))

gremlin_connection = 'ws://localhost:8182/gremlin'
traversal_source = 'g'


# block -> transactions -> internal transactions (sequential)
# type - call , create , suicide
# error

def address_nodes(g, fromAddress, isFromContract, toAddress, isToContract, txId):
    frA = g.V().hasLabel("account").has('addr', fromAddress).toList()

    if len(frA) == 0:
        frA = g.addV("account").property('addr', fromAddress).property('contract', isFromContract)
        frA.addE("from").to(g.V().hasId(txId)).toList()
    else:
        g.V().hasId(frA[0].id).addE("from").to(g.V().hasId(txId)).toList()

    if toAddress is not None:
        toA = g.V().hasLabel("account").has('addr', toAddress).toList()

        if len(toA) == 0:
            toA = g.addV("account").property('addr', toAddress).property('contract', isToContract)
            g.V().hasId(txId).addE("to").to(toA).toList()
        else:
            g.V().hasId(txId).addE("to").to(g.V().hasId(toA[0].id)).toList()


def decide_on_action(g, tx, txId, error):
    if tx['type'] == 'call':
        address_nodes(g, tx['action']['from'], False, tx['action']['to'], True, txId)
    elif tx['type'] == 'create':
        if error:
            address_nodes(g, tx['action']['from'], False, None, False, txId)
        else:
            address_nodes(g, tx['action']['from'], False, tx['result']['address'], True, txId)
    elif tx['type'] == 'suicide':
        address_nodes(g, tx['action']['address'], False, tx['action']['refundAddress'], False, txId)
    else:
        address_nodes(g, tx['action']['from'], False, tx['result']['address'], True, txId)


def parall(blockNumbers):
    blockNumbers = blockNumbers.tolist()
    g = Graph().traversal().withRemote(DriverRemoteConnection(gremlin_connection, traversal_source))
    for blockNumber in blockNumbers:
        # blockE = web3.eth.getBlock(blockNumber, True)
        block = web3._requestManager.request_blocking("trace_block", [web3.fromDecimal(blockNumber)])

        count = len(block)
        if count > 0:
            blockEx = g.V().hasLabel("block").has('number', blockNumber).toList()
            count_ = len(blockEx)

            if count_ == 0:
                blockV = g.addV('block').property('number', blockNumber).next()
            else:
                blockV = blockEx[0]

            # Add transactions
            subtraces = 0
            prevTx = None
            for index in range(count):
                tx = block[index]

                if 'error' in tx:
                    error = True
                else:
                    error = False

                # Internal transaction
                if subtraces > 0:
                    subtraces -= 1
                    temp = g.addV('intx').property('hash', tx['transactionHash']).property('type', tx['type'])
                    if error:
                        temp.property('error', True)
                    edge = g.V().hasId(prevTx).addE('internal').to(temp).toList()

                else:
                    temp = g.addV('tx').property('hash', tx['transactionHash']).property('type', tx['type'])
                    if error:
                        temp.property('error', tx['error'])
                    edge = g.V().hasId(blockV.id).addE('include').to(temp).toList()

                decide_on_action(g, tx, edge[0].inV.id, error)

                if tx['subtraces'] > 0:
                    subtraces = tx['subtraces']
                    prevTx = edge[0].inV.id


# toBlock = web3.eth.blockNumber
toBlock = 50000
chunks = np.array_split(np.array(range(toBlock)), 4)

start = time.time()

worker1 = Thread(target=parall, args=([chunks[0]]))
worker3 = Thread(target=parall, args=([chunks[1]]))
worker2 = Thread(target=parall, args=([chunks[2]]))
worker4 = Thread(target=parall, args=([chunks[3]]))

worker1.start()
worker2.start()
worker3.start()
worker4.start()

worker1.join()
worker2.join()
worker3.join()
worker4.join()

end = time.time()
print(end - start, "seconds elapsed")
