#include <boost/python.hpp>
#include "Blockchain.h"

using namespace boost::python;

BOOST_PYTHON_MODULE(ethersci)
{
    //BLOCK
    class_< Block >("Block")
      .def_readwrite("number", &Block::number)
      .def_readwrite("hash", &Block::hash)
      .def_readwrite("parentHash", &Block::parentHash)
      .def_readwrite("sha3Uncles", &Block::sha3Uncles)
      .def_readwrite("logsBloom", &Block::logsBloom)
      .def_readwrite("transactionsRoot", &Block::transactionsRoot)
      .def_readwrite("stateRoot", &Block::stateRoot)
      .def_readwrite("receiptsRoot", &Block::receiptsRoot)
      .def_readwrite("miner", &Block::miner)
      .def_readwrite("difficulty", &Block::difficulty)
      .def_readwrite("totalDifficulty", &Block::totalDifficulty)
      .def_readwrite("extraData", &Block::extraData)
      .def_readwrite("size", &Block::size);
    //BLOCKCHAIN
    // Overloaded methods
    Block (Blockchain::*getBlock_number)(int) = &Blockchain::getBlock;
    Block (Blockchain::*getBlock_hash)(std::string) = &Blockchain::getBlock;

    class_< Blockchain >("Blockchain", init<std::string>())
      .def("getBlock", getBlock_number)
      .def("getBlock", getBlock_hash)
      .def("size", &Blockchain::size);
}