cmake_minimum_required(VERSION 2.8.9)


project(ethereum_parser)

#Define project name
set(PROJECT_NAME "ethereum_parser")
#Bring the headers, such as Block.h into the project
include_directories(include)


set(IS_LIBRARY FALSE)
if(NOT ${IS_LIBRARY})
    #Set SOURCES with all cpp files in src directory excluding main file
    set(SOURCES
            src/utils.cpp
            src/rlp.cpp
            src/parser.cpp
            src/block.cpp
            src/address_recover.cpp
            src/account.cpp
     )

    set(MAIN src/main.cpp)
    add_executable(${PROJECT_NAME} ${MAIN} ${SOURCES})
    target_link_libraries (${PROJECT_NAME} leveldb cryptopp secp256k1 gtest ${BASE_LIBS})

    file(GLOB TEST_SRCS "test/*.cpp")
    add_executable(test_parser ${SOURCES} ${TEST_SRCS} )
    target_link_libraries (test_parser leveldb cryptopp secp256k1 gtest ${BASE_LIBS})


endif()
