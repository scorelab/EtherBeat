#include <sstream>
std::string int_to_hex(int a){
    std::stringstream ss;
    ss << std::hex << a;
    return "0x"+ss.str();
}
int hex_to_int(std::string a){
    return std::stoul(a, nullptr, 16);
}