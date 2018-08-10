/*
 * Created by prabushitha on 6/6/18.
 * Copyright [2018] <ScoreLab Organization>
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
    uint8_t prefix[3] = {1, 2, 3};
    uint8_t middle[2] = {1, 3};
    uint8_t suffix[2] = {4, 5};
    std::string key = getKeyString(prefix, middle, suffix, 3, 2, 2);
    EXPECT_EQ(key, "\001\002\003\001\003\004\005");
}

TEST(HexString, ByteVectorToHex) {
    // bytes array to hex string
    std::vector<uint8_t> bytes_arr;
    bytes_arr.insert(bytes_arr.end(), 0);
    bytes_arr.insert(bytes_arr.end(), 0);
    bytes_arr.insert(bytes_arr.end(), 3);
    bytes_arr.insert(bytes_arr.end(), 28);

    std::string str = hexStr(&bytes_arr[0], bytes_arr.size());
    EXPECT_EQ(str, "0000031c");
}

TEST(HexString, ByteVectorToHexEmpty) {
    // bytes array to hex string
    std::vector<uint8_t> bytes_arr;

    std::string str = hexStr(&bytes_arr[0], bytes_arr.size());
    EXPECT_EQ(str, "");
}
TEST(HexString, ByteVectorToHexZero) {
    // bytes array to hex string
    std::vector<uint8_t> bytes_arr;
    bytes_arr.insert(bytes_arr.end(), 0);

    std::string str = hexStr(&bytes_arr[0], bytes_arr.size());
    EXPECT_EQ(str, "00");
}

TEST(bytesVectorToInt, EmptyByteVector) {
    // bytes array to hex string
    std::vector<uint8_t> bytes_arr;

    size_t val = bytesVectorToInt(bytes_arr);

    EXPECT_EQ(val, 0);
}

TEST(bytesVectorToInt, ByteVector) {
    // bytes array to hex string
    std::vector<uint8_t> bytes_arr;
    bytes_arr.insert(bytes_arr.end(), 0);
    bytes_arr.insert(bytes_arr.end(), 0);
    bytes_arr.insert(bytes_arr.end(), 0);
    bytes_arr.insert(bytes_arr.end(), 0);
    bytes_arr.insert(bytes_arr.end(), 0);
    bytes_arr.insert(bytes_arr.end(), 0);
    bytes_arr.insert(bytes_arr.end(), 1);
    bytes_arr.insert(bytes_arr.end(), 3);

    size_t val = bytesVectorToInt(bytes_arr);

    EXPECT_EQ(val, 259);
}

TEST(BytesVector, EmptyByteString) {
    std::string str = "";
    std::vector<uint8_t> bytes_arr = getByteVector(str);
    EXPECT_EQ(bytes_arr.size(), 0);
}

TEST(BytesVector, ValidByteString) {
    std::string str = "a23";
    std::vector<uint8_t> bytes_arr = getByteVector(str);
    EXPECT_EQ(bytes_arr.size(), 3);

    EXPECT_EQ(bytes_arr[0], 97);
    EXPECT_EQ(bytes_arr[1], 50);
    EXPECT_EQ(bytes_arr[2], 51);
}

TEST(Keccak256, OneElement) {
    std::vector<uint8_t> inp;
    inp.insert(inp.end(), 1);
    std::vector<uint8_t> bytes_arr = keccak_256(inp);
    std::string hashHex = hexStr(&bytes_arr[0], bytes_arr.size());

    EXPECT_EQ(hashHex, "5fe7f977e71dba2ea1a68e21057beebb9be2ac30c6410aa38d4f3fbe41dcffd2");
}

TEST(Hex2Bytes, emptyHex) {
    std::string hex_str = "";
    std::vector<uint8_t> bytes_arr = hex_to_bytes(hex_str);

    EXPECT_EQ(bytes_arr.size(), 0);
}

TEST(Hex2Bytes, validHex) {
    std::string hex_str = "00000012";
    std::vector<uint8_t> bytes_arr = hex_to_bytes(hex_str);

    EXPECT_EQ(bytes_arr.size(), 4);
    EXPECT_EQ(bytes_arr[0], 0);
    EXPECT_EQ(bytes_arr[1], 0);
    EXPECT_EQ(bytes_arr[2], 0);
    EXPECT_EQ(bytes_arr[3], 18);
}

TEST(Remove0xFromString, HexWithLeading0x) {
    std::string hex_str = "0x00000012";
    std::string res = remove0xFromString(hex_str);

    EXPECT_EQ(res, "00000012");
}
TEST(Remove0xFromString, InvalidHexWithMid0x) {
    std::string hex_str = "000x000012";
    std::string res = remove0xFromString(hex_str);

    EXPECT_EQ(res, "000x000012");
}

TEST(Remove0xFromString, HexWithNo0x) {
    std::string hex_str = "00000012";
    std::string res = remove0xFromString(hex_str);

    EXPECT_EQ(res, "00000012");
}
