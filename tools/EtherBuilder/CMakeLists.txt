cmake_minimum_required(VERSION 3.10)
#Define project name
set(PROJECT_NAME "EtherBuilder")

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)

# include the extractor library in tools/EtherExtractor
link_directories(
        ../EtherExtractor/build
)
include_directories(
        include
        ../EtherExtractor/include
)
set(SOURCES
        src/block_store.cpp
)
set(HEADERS
        include/block_store.h
        include/sql_statements.h
        include/timing.h
        )


# create the executable
add_executable(${PROJECT_NAME} src/main.cpp ${SOURCES} ${HEADERS})
target_link_libraries (${PROJECT_NAME} ethereum_extractor sqlite3 pthread rocksdb leveldb cryptopp secp256k1 gtest ${BASE_LIBS})

