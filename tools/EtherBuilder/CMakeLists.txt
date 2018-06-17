cmake_minimum_required(VERSION 3.10)
#Define project name
set(PROJECT_NAME "EtherBuilder")

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)

# include the parser library in tools/parser
link_directories(
        ../parser/build
)
include_directories(../parser/include include)

# create the executable
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries (${PROJECT_NAME} rocksdb leveldb cryptopp secp256k1 gtest ethereum_parser sqlite3 ${BASE_LIBS})
