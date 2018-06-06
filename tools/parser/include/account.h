/*
 * Created by prabushitha on 5/29/18.
*/
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