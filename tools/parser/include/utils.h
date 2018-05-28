#include <vector>
#include <string>

std::string getKeyString(uint64_t, uint8_t [], uint8_t [], int , int);
void print_bytes (std::string, std::string heading="Bytes");
void print_bytes (std::vector<uint8_t> val, std::string heading="Bytes");
std::string hexStr(unsigned char *, int);
int hexStringToInt(std::string);
int bytesVectorToInt(std::vector<uint8_t> bytes_arr);
std::vector<uint8_t> getByteVector(std::string byteString);

std::vector<uint8_t> sha3(std::vector<uint8_t > &input, int digestSize);
std::vector<uint8_t> keccak_256(std::vector<uint8_t > &input);



