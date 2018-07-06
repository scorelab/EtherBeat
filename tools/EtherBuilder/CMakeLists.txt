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
        include/test_sqlite.h
)


# create the executable
add_executable(${PROJECT_NAME} main.cpp ${SOURCES} ${HEADERS})

target_link_libraries (${PROJECT_NAME} rocksdb leveldb cryptopp secp256k1 gtest ethereum_extractor sqlite3 ${BASE_LIBS})

# researching related
add_executable(test_sqlite test_sqlite.cpp ${SOURCES} ${HEADERS})
target_link_libraries (test_sqlite rocksdb leveldb cryptopp secp256k1 gtest ethereum_extractor sqlite3 ${BASE_LIBS})