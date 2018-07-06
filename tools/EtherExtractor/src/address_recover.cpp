/*
 * Created by prabushitha on 5/27/18.
*/
#include "address_recover.h"
#include "utils.h"

secp256k1_context const* getCtx()
{
	static std::unique_ptr<secp256k1_context, decltype(&secp256k1_context_destroy)> s_ctx{
		secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY),
		&secp256k1_context_destroy
	};
	return s_ctx.get();
}


// Message is 32 bit hash of the tx/message
std::vector<uint8_t> recover(uint8_t v_new, std::vector<uint8_t> r, std::vector<uint8_t> s, std::vector<uint8_t> _message)
{
	std::vector<uint8_t> public_key;
	int v = v_new-27;

	if (v > 3 || v < 0){
		printf("Invalid Recid: %d\n", v);
		return {};
	}

	// A signature: 65 bytes: r: [0, 32), s: [32, 64), v: 64.
	//concat r and s with 32 in size (leading 0s if size mismatch)
	std::vector<uint8_t> signature;
	signature.reserve( 65 );                // preallocate memory
	// left pad 0 for r
	int i=0;
	for(i=0; i<32-r.size(); i++){
		signature.insert( signature.end(), 0 );
	}
	signature.insert( signature.end(), r.begin(), r.end() );

	// left pad 0 for s
	for(i=0; i<32-s.size(); i++){
		signature.insert( signature.end(), 0 );
	}
	signature.insert( signature.end(), s.begin(), s.end() );
	signature[65] = v_new;

	auto* ctx = getCtx();
	secp256k1_ecdsa_recoverable_signature rawSig;
	if (!secp256k1_ecdsa_recoverable_signature_parse_compact(ctx, &rawSig, (uint8_t *)&signature[0], v)){
		printf("No secp256k1_ecdsa_recoverable_signature\n");
		return {};
	}

	secp256k1_pubkey rawPubkey;
	if (!secp256k1_ecdsa_recover(ctx, &rawPubkey, &rawSig, (uint8_t *)&_message[0])){
		printf("Couldn't recover public key\n");
		return {};
	}


	std::array<uint8_t, 65> serializedPubkey;
	size_t serializedPubkeySize = serializedPubkey.size();
	secp256k1_ec_pubkey_serialize(
			ctx, serializedPubkey.data(), &serializedPubkeySize,
			&rawPubkey, SECP256K1_EC_UNCOMPRESSED
	);
	// // assert(serializedPubkeySize == serializedPubkey.size());
	// Expect single byte header of value 0x04 -- uncompressed public key.
	// // assert(serializedPubkey[0] == 0x04);
	// Create the Public key skipping the header.
	return std::vector<uint8_t> (serializedPubkey.begin()+1, serializedPubkey.end());
}


std::vector<uint8_t> publicKeyToAddress(std::vector<uint8_t> public_key) {
    std::vector<uint8_t> pubkey_keccak = keccak_256(public_key);

    if(pubkey_keccak.size() == 32) {
        return std::vector<uint8_t> (pubkey_keccak.begin()+12, pubkey_keccak.end());
    }
    std::vector<uint8_t> address (0, 40);
    return address;
}
