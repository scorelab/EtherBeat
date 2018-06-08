/*
 * Created by prabushitha on 5/19/18.
*/
#include <vector>
#include <string>


// returns a byte string (used to create the leveldb key)
std::string getKeyString(uint8_t prefix[], uint8_t middle[], uint8_t suffix[], int prefixSize=0, int middleSize=0, int suffixSize=0);

// returns a byte vector from a string (to convert the response from leveldb to bytes)
std::vector<uint8_t> getByteVector(std::string byteString);


// converts a number to big endian notation with 8 bytes
std::vector<uint8_t> toBigEndianEightBytes(uint64_t number);

// converts an byte array (pointed to the 1st byte address) to a hexadecimal notation string
std::string hexStr(unsigned char *, int);

//
int hexStringToInt(std::string);

// assembles a byte vector to its integer notation
size_t bytesVectorToInt(std::vector<uint8_t> bytes_arr);

// assembles a byte vector to its string notation
std::string bytesVectorToHexString(std::vector<uint8_t> attribute);

// returns keccak_256 hash (in byte form) to a given bytes vector
std::vector<uint8_t> keccak_256(std::vector<uint8_t > &input);

// returns a byte vector from hexadecimal notation
std::vector<uint8_t> hex_to_bytes(std::string const& hex);

// returns a string without leading 0x (basically to remove 0x from user given hexadecimal string)
std::string remove0xFromString(std::string hex_str);

// printing
void print_bytes (std::string, std::string heading="Bytes");
void print_bytes (std::vector<uint8_t> val, std::string heading="Bytes");

