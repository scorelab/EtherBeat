/*
 * Created by prabushitha on 5/29/18.
 * Copyright [2018] <ScoreLab Organization>
*/
#ifndef TOOLS_ETHEREXTRACTOR_INCLUDE_ACCOUNT_H_
#define TOOLS_ETHEREXTRACTOR_INCLUDE_ACCOUNT_H_

#include <vector>
#include <cstdint>

// Account
class Account {
 public:
    std::vector<uint8_t> address;
    std::vector<uint8_t> balance;

    int blockHeight;

    void print();
};

#endif  // TOOLS_ETHEREXTRACTOR_INCLUDE_ACCOUNT_H_
