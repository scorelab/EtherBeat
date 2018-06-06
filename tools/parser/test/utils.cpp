/*
 * Created by prabushitha on 6/6/18.
*/
#include <gtest/gtest.h>
#include "utils.h"

TEST(Test_Big_Endian, SmallNumber) {
    std::vector<uint8_t> bigendian = toBigEndianEightBytes(28);
    EXPECT_EQ(bigendian.size(), 8);
    EXPECT_EQ(bigendian[0], 0);
    EXPECT_EQ(bigendian[1], 0);
    EXPECT_EQ(bigendian[2], 0);
    EXPECT_EQ(bigendian[3], 0);
    EXPECT_EQ(bigendian[4], 0);
    EXPECT_EQ(bigendian[5], 0);
    EXPECT_EQ(bigendian[6], 0);
    EXPECT_EQ(bigendian[7], 0x1C);
}

TEST(Test_Big_Endian, MediumNumber) {
    std::vector<uint8_t> bigendian = toBigEndianEightBytes(2000);
    EXPECT_EQ(bigendian.size(), 8);
    EXPECT_EQ(bigendian[0], 0x00);
    EXPECT_EQ(bigendian[1], 0x00);
    EXPECT_EQ(bigendian[2], 0x00);
    EXPECT_EQ(bigendian[3], 0x00);
    EXPECT_EQ(bigendian[4], 0x00);
    EXPECT_EQ(bigendian[5], 0x00);
    EXPECT_EQ(bigendian[6], 0x07);
    EXPECT_EQ(bigendian[7], 0xD0);
}

TEST(Test_Big_Endian, LargeNumber) {
    std::vector<uint8_t> bigendian = toBigEndianEightBytes(20000000);
    EXPECT_EQ(bigendian.size(), 8);
    EXPECT_EQ(bigendian[0], 0x00);
    EXPECT_EQ(bigendian[1], 0x00);
    EXPECT_EQ(bigendian[2], 0x00);
    EXPECT_EQ(bigendian[3], 0x00);
    EXPECT_EQ(bigendian[4], 0x01);
    EXPECT_EQ(bigendian[5], 0x31);
    EXPECT_EQ(bigendian[6], 0x2D);
    EXPECT_EQ(bigendian[7], 0x00);
}

TEST(Test_Big_Endian, VeryLargeNumber) {
    std::vector<uint8_t> bigendian = toBigEndianEightBytes(2000000000000000);
    EXPECT_EQ(bigendian.size(), 8);
    EXPECT_EQ(bigendian[0], 0);
    EXPECT_EQ(bigendian[1], 0x07);
    EXPECT_EQ(bigendian[2], 0x1A);
    EXPECT_EQ(bigendian[3], 0xFD);
    EXPECT_EQ(bigendian[4], 0x49);
    EXPECT_EQ(bigendian[5], 0x8D);
    EXPECT_EQ(bigendian[6], 0x00);
    EXPECT_EQ(bigendian[7], 0x00);
}



TEST(Keystring, PrefixMiddleSuffix) {
    uint8_t prefix[3] = {1,2,3};
    uint8_t middle[2] = {1,3};
    uint8_t suffix[2] = {4,5};
    std::string key = getKeyString(prefix, middle, suffix, 3, 2, 2);
    EXPECT_EQ(key, "\001\002\003\001\003\004\005");
}

TEST(HexString, PrefixMiddleSuffix) {
    // bytes array to hex string
    std::vector<uint8_t> bytes_arr;
    bytes_arr.insert(bytes_arr.end(), 0);
    bytes_arr.insert(bytes_arr.end(), 0);
    bytes_arr.insert(bytes_arr.end(), 3);
    bytes_arr.insert(bytes_arr.end(), 28);

    std::string str = hexStr(&bytes_arr[0], bytes_arr.size());
    EXPECT_EQ(str, "0000031c");
}