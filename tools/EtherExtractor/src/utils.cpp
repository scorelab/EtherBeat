/*
 * Created by prabushitha on 5/6/18.
 * Copyright [2018] <ScoreLab Organization>
*/
#include <sstream>
#include <vector>
#include <cryptopp/keccak.h>
#include <cstdio>

constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                           '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};


// Returns Hex String for a given byte character location
std::string hexStr(unsigned char *data, int len) {
    std::string s(len * 2, ' ');
    for (int i = 0; i < len; ++i) {
        s[2 * i] = hexmap[(data[i] & 0xF0) >> 4];
        s[2 * i + 1] = hexmap[data[i] & 0x0F];
    }
    return s;
}

// hex to bytes
std::vector<uint8_t> hex_to_bytes(std::string const &hex) {
    std::vector<uint8_t> bytes;
    bytes.reserve(hex.size() / 2);
    for (std::string::size_type i = 0, i_end = hex.size(); i < i_end; i += 2) {
        unsigned byte;
        std::istringstream hex_byte(hex.substr(i, 2));
        hex_byte >> std::hex >> byte;
        bytes.push_back(static_cast<uint8_t>(byte));
    }
    return bytes;
}

// Print bytes for a given byte string
void print_bytes(std::string val, std::string heading = "Bytes") {
    printf("%s : [", heading.c_str());
    for (char &c : val) {
        uint8_t byteval = c;
        printf("%d ", byteval);
    }
    printf("]\n\n");
}

// Print bytes for a given byte vector
void print_bytes(std::vector<uint8_t> val, std::string heading = "Bytes") {
    printf("%s : [", heading.c_str());
    for (uint8_t c : val) {
        printf("%d ", c);
    }
    printf("]\n\n");
}

int hexStringToInt(std::string hexString) {
    int x;
    std::stringstream ss;
    ss << std::hex << hexString;
    ss >> x;
    return x;
}

double hexastr2double(const std::string &s) {
    double d = 0.0;

    std::sscanf(s.c_str(), "%lA", &d);

    return d;
}

size_t bytesVectorToInt(std::vector<uint8_t> bytes_arr) {
    size_t dest = 0;
    for (unsigned i = 0; i < bytes_arr.size(); ++i) {
        dest *= 256;
        dest += bytes_arr[i];
    }
    return dest;
}

std::string bytesVectorToHexString(std::vector<uint8_t> attribute) {
    if (attribute.empty()) return "";
    std::string attr_string = hexStr(&attribute[0], static_cast<int>(attribute.size()));
    return attr_string;
}

unsigned long bytesVectorToLong(std::vector<uint8_t> bytes_arr) {
    if (bytes_arr.empty())
        return 0;
    std::stringstream str;
    std::string hexGasPrice = bytesVectorToHexString(bytes_arr);
    str << hexGasPrice;
    unsigned long value;
    str >> std::hex >> value;
    return value;
}

std::vector<uint8_t> getByteVector(std::string byteString) {
    std::vector<uint8_t> contents;
    int i;
    for (i = 0; i < byteString.length(); i++) {
        contents.insert(contents.end(), (uint8_t) byteString[i]);
    }
    return contents;
}

std::vector<uint8_t> toBigEndianEightBytes(uint64_t number) {
    /*
        Convert number to bigendian notation array of uint8_t
    */
    uint8_t *p = reinterpret_cast<uint8_t *> (&number);

    // we need a copy
    std::vector<uint8_t> result;

    result.reserve(8);
    result.insert(result.end(), (number >> 56) & 0xFF);
    result.insert(result.end(), (number >> 48) & 0xFF);
    result.insert(result.end(), (number >> 40) & 0xFF);
    result.insert(result.end(), (number >> 32) & 0xFF);
    result.insert(result.end(), (number >> 24) & 0xFF);
    result.insert(result.end(), (number >> 16) & 0xFF);
    result.insert(result.end(), (number >> 8) & 0xFF);
    result.insert(result.end(), number & 0xFF);
    /*
    result[0] = (number >> 56) & 0xFF;
    result[1] = (number >> 48) & 0xFF;
    result[2] = (number >> 40) & 0xFF;
    result[3] = (number >> 32) & 0xFF;
    result[4] = (number >> 24) & 0xFF;
    result[5] = (number >> 16) & 0xFF;
    result[6] = (number >> 8) & 0xFF;
    result[7] = number & 0xFF;
    */
    return result;
}

std::string getKeyString(uint8_t prefix[], uint8_t middle[], uint8_t suffix[], int prefixSize = 0, int middleSize = 0,
                         int suffixSize = 0) {
    std::vector<uint8_t> hexval;

    // Example append prefix 'h' and suffix 'n' to the hexval bytes
    // hexval.insert(hexval.begin(), 104); // hex = 68 0 0 0 0 0 0 0 28
    // hexval.insert(hexval.end(), 110); // hex = 68 0 0 0 0 0 0 0 28 6e

    // We'll generalize above to append any number of middle , prefixes and suffixes
    int i;
    for (i = 0; i < middleSize; i++) {
        hexval.insert(hexval.end(), middle[i]);
    }

    for (i = prefixSize - 1; i >= 0; --i) {
        hexval.insert(hexval.begin(), prefix[i]);
    }

    for (i = 0; i < suffixSize; ++i) {
        hexval.insert(hexval.end(), suffix[i]);
    }


    // create the key string
    std::string keyString(hexval.begin(), hexval.end());
    return keyString;
}

std::string remove0xFromString(std::string hex_str) {
    if (hex_str.length() > 1 && hex_str.substr(0, 2) == "0x") {
        return hex_str.substr(2, hex_str.length() - 2);
    }
    return hex_str;
}

// Keccak 256 hash
CryptoPP::Keccak_256 _keccak;

std::vector<uint8_t> keccak_256(std::vector<uint8_t> &input) {
    std::vector<uint8_t> digest(_keccak.DigestSize());
    _keccak.CalculateDigest(digest.data(), input.data(), input.size());
    return digest;
}

