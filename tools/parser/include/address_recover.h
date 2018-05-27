#include <memory>
#include <secp256k1.h>
#include <secp256k1_recovery.h>
#include <vector>

secp256k1_context const* getCtx();
std::vector<uint8_t> recover(std::vector<uint8_t> _sig, std::vector<uint8_t> _message);
std::vector<uint8_t> publicKeyToAddress(std::vector<uint8_t> public_key);