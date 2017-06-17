from web3 import Web3, KeepAliveRPCProvider, IPCProvider
from gremlin_python.process.graph_traversal import __
from gremlin_python.driver.driver_remote_connection import DriverRemoteConnection
from gremlin_python.structure.graph import Graph

import time
import numpy
from threading import Thread
from tornado import ioloop
from multiprocessing.dummy import Pool as ThreadPool

web3 = Web3(KeepAliveRPCProvider(host='localhost', port='8545'))

gremlin_connection = 'ws://localhost:8182/gremlin'
traversal_source = 'g'


# g = Graph().traversal().withRemote(DriverRemoteConnection(gremlin_connection, traversal_source))


# tr = web3.eth.getTransaction("0x74dbf22a0eddde8f94f09c3d56aaac2ae2de896a7aa5f95f1257c012f269ce92")
# sss = web3._requestManager.request_blocking("trace_block", [web3.fromDecimal(11)])
# ds = web3.fromDecimal(11)

# block -> transactions -> internal transactions (sequential)
# type - call , create , suicide
# error ?

def addressNodes(g, fromAddress, isFromContract, toAddress, isToContract, txId):
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


def decideOnAction(g, tx, edge, error):
    if tx['type'] == 'call':
        addressNodes(g, tx['action']['from'], False, tx['action']['to'], True, edge[0].outV.id)
    elif tx['type'] == 'create':
        if error:
            addressNodes(g, tx['action']['from'], False, None, False, edge[0].outV.id)
        else:
            addressNodes(g, tx['action']['from'], False, tx['result']['address'], True, edge[0].outV.id)
    elif tx['type'] == 'suicide':
        addressNodes(g, tx['action']['address'], False, tx['action']['refundAddress'], False, edge[0].outV.id)
    else:
        addressNodes(g, tx['action']['from'], False, tx['result']['address'], True, edge[0].outV.id)


def parall(blockNumbers, loop):
    blockNumbers = blockNumbers.tolist()
    g = Graph().traversal().withRemote(DriverRemoteConnection(gremlin_connection, traversal_source, loop=loop))
    for blockNumber in blockNumbers:
        # blockE = web3.eth.getBlock(blockNumber, True)
        block = web3._requestManager.request_blocking("trace_block", [web3.fromDecimal(blockNumber)])

        first = True
        count = len(block)
        blockV = None
        if count > 0:
            # print(blockNumber)
            if first:
                blockV = g.addV('block').property('number', blockNumber).next()
                first = False

            # Add transactions

            subtraces = 0
            prevTx = None
            for index in range(len(block)):
                tx = block[index]

                if 'error' in tx:
                    error = True
                else:
                    error = False

                if subtraces > 0:
                    subtraces -= 1
                    temp = g.addV('intx').property('hash', tx['transactionHash']).property('type', tx['type'])
                    if error:
                        temp.property('error', tx['error'])
                    edge = g.V().hasId(prevTx.id).addE('internal').to(temp).toList()

                else:
                    temp = g.addV('tx').property('hash', tx['transactionHash']).property('type', tx['type'])
                    if error:
                        temp.property('error', tx['error'])
                    edge = g.V().hasId(blockV.id).addE('include').to(temp).toList()

                decideOnAction(g, tx, edge, error)

                if tx['subtraces'] > 0:
                    subtraces = tx['subtraces']
                    prevTx = edge[0].outV

                    # print("finished", blockNumber)


toBlock = web3.eth.blockNumber
toBlock = 50000
ls = range(toBlock)

start = time.time()

loop1 = ioloop.IOLoop()
loop2 = ioloop.IOLoop()
loop3 = ioloop.IOLoop()
loop4 = ioloop.IOLoop()

chunks = numpy.array_split(numpy.array(ls), 4)
worker1 = Thread(target=parall, args=(chunks[0], loop1))
worker3 = Thread(target=parall, args=(chunks[1], loop3))
worker2 = Thread(target=parall, args=(chunks[2], loop2))
worker4 = Thread(target=parall, args=(chunks[3], loop4))

worker1.start()
worker2.start()
worker3.start()
worker4.start()

worker1.join()
worker2.join()
worker3.join()
worker4.join()
# pool = ThreadPool(4)
# pool.map(parall, range(toBlock))
#
# pool.close()
# pool.join()

end = time.time()
print(end - start, "seconds elapsed")
