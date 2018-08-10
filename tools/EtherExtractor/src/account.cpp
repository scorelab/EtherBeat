/*
 * Created by prabushitha on 5/29/18.
 * Copyright [2018] <ScoreLab Organization>
*/
#include <account.h>
#include <utils.h>
#include <string>

void Account::print() {
    printf("Address: %s \n",  hexStr((unsigned char *)&address[0], address.size()).c_str());
    printf("Balance: %s \n",  hexStr((unsigned char *)&balance[0], balance.size()).c_str());
}
