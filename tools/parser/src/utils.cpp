#include <sstream>
#include <vector>
#include <cryptopp/keccak.h>

constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                           '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};


// Returns Hex String for a given byte character location
std::string hexStr(unsigned char *data, int len)
{
  std::string s(len * 2, ' ');
  for (int i = 0; i < len; ++i) {
    s[2 * i]     = hexmap[(data[i] & 0xF0) >> 4];
    s[2 * i + 1] = hexmap[data[i] & 0x0F];
  }
  return s;
}

// Print bytes for a given byte string
void print_bytes (std::string val, std::string heading="Bytes"){
    printf("%s : [", heading.c_str());
    for(char& c : val) {
            uint8_t byteval = c;
            printf("%d ", byteval);
    }
    printf("]\n\n");
}

// Print bytes for a given byte vector
void print_bytes (std::vector<uint8_t> val, std::string heading="Bytes"){
    printf("%s : [", heading.c_str());
    for(uint8_t c : val) {
            printf("%d ", c);
    }
    printf("]\n\n");
}

int hexStringToInt(std::string hexString){
    int x;
    std::stringstream ss;
    ss << std::hex << hexString;
    ss >> x;
    return x;
}

size_t bytesVectorToInt(std::vector<uint8_t> bytes_arr) {
    size_t dest = 0;
    for (unsigned i = 0; i < bytes_arr.size(); ++i) {
        dest *= 256;
        dest += bytes_arr[i];
    }
    return dest;
}

std::vector<uint8_t> getByteVector(std::string byteString){
    std::vector<uint8_t> contents;
    int i;
    for(i=0; i<byteString.length(); i++) {
        contents.insert(contents.end(), (uint8_t)byteString[i]);
    }
    return contents;
}


std::string getKeyString(uint64_t blockNumber, uint8_t prefix[], uint8_t suffix[], int prefixSize = 1, int suffixSize = 1){

    /*
        Convert blockNumber to bigendian notation array of uint8_t
    */
    uint8_t *p = (uint8_t *)&blockNumber;
    //if you need a copy
    uint8_t result[8];

    result[0] = (blockNumber >> 56) & 0xFF;
    result[1] = (blockNumber >> 48) & 0xFF;
    result[2] = (blockNumber >> 40) & 0xFF;
    result[3] = (blockNumber >> 32) & 0xFF;
    result[4] = (blockNumber >> 24) & 0xFF;
    result[5] = (blockNumber >> 16) & 0xFF;
    result[6] = (blockNumber >> 8) & 0xFF;
    result[7] = blockNumber & 0xFF;

    std::vector<uint8_t> hexval; // = {0,0,0,0,0,0,0, blockNumber}; // hex = 0 0 0 0 0 0 0 28
    for(uint8_t c : result) {
        hexval.insert(hexval.end(), c);
    }

    // Example append prefix 'h' and suffix 'n' to the hexval bytes
    // hexval.insert(hexval.begin(), 104); // hex = 68 0 0 0 0 0 0 0 28
    // hexval.insert(hexval.end(), 110); // hex = 68 0 0 0 0 0 0 0 28 6e
    // We'll generalize above to append any number of prefixes and suffixes
    int i;
    for (i = prefixSize-1; i >= 0; --i) {
        hexval.insert(hexval.begin(), prefix[i]);
    }
    for (i = 0; i < suffixSize; ++i) {
        hexval.insert(hexval.end(), suffix[i]);
    }


    // create the key string
    std::string keyString(hexval.begin(), hexval.end());
    return keyString;
}

// Keccak 256 hash
CryptoPP::Keccak_256 _keccak;
std::vector<uint8_t> keccak_256(std::vector<uint8_t> &input) {

    std::vector<uint8_t> digest(_keccak.DigestSize());
    _keccak.CalculateDigest(digest.data(), input.data(), input.size());

    return digest;
}
