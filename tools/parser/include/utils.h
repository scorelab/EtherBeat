/*
 * Created by prabushitha on 5/19/18.
*/
#include <vector>
#include <string>

std::vector<uint8_t> toBigEndianEightBytes(uint64_t number);
std::string getKeyString(uint8_t prefix[], uint8_t middle[], uint8_t suffix[], int prefixSize=0, int middleSize=0, int suffixSize=0);
void print_bytes (std::string, std::string heading="Bytes");
void print_bytes (std::vector<uint8_t> val, std::string heading="Bytes");
std::string hexStr(unsigned char *, int);

int hexStringToInt(std::string);
size_t bytesVectorToInt(std::vector<uint8_t> bytes_arr);

std::vector<uint8_t> getByteVector(std::string byteString);

std::vector<uint8_t> keccak_256(std::vector<uint8_t > &input);

std::vector<uint8_t> hex_to_bytes(std::string const& hex);

std::string remove0xFromString(std::string hex_str);


