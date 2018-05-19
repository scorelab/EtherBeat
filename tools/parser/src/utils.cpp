#include <sstream>
#include <vector>

constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                           '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

std::string hexStr(unsigned char *data, int len)
{
  std::string s(len * 2, ' ');
  for (int i = 0; i < len; ++i) {
    s[2 * i]     = hexmap[(data[i] & 0xF0) >> 4];
    s[2 * i + 1] = hexmap[data[i] & 0x0F];
  }
  return s;
}

void print_bytes (std::string val){
    printf("Bytes : [");
    for(char& c : val) {
            uint8_t byteval = c;
            printf("%d ", byteval);
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

    int i;
    for (i = prefixSize-1; i >= 0; --i) {
        hexval.insert(hexval.begin(), prefix[i]);
    }
    for (i = 0; i < suffixSize; ++i) {
        hexval.insert(hexval.end(), suffix[i]);
    }
    // append prefix 'h' and 'n' to the hexval bytes
    // hexval.insert(hexval.begin(), 104); // hex = 68 0 0 0 0 0 0 0 28
    // hexval.insert(hexval.end(), 110); // hex = 68 0 0 0 0 0 0 0 28 6e

    // create the key string
    std::string keyString(hexval.begin(), hexval.end());
    return keyString;
}
