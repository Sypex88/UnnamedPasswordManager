#include <vector>
#include <string>
#include <cstring>
#include <stdexcept>
#include <sodium.h>
#include "VaultCrypto.h"

std::string VaultCrypto::encryptJSON(
const std::string& json,
const std::string& masterPassword
) {
    unsigned char salt[crypto_pwhash_SALTBYTES];
    randombytes_buf(salt, sizeof salt);

    unsigned char key[crypto_secretbox_KEYBYTES];
    crypto_pwhash(
        key, sizeof key,
        masterPassword.c_str(),
        masterPassword.size(),
        salt,
        crypto_pwhash_OPSLIMIT_MODERATE,
        crypto_pwhash_MEMLIMIT_MODERATE,
        crypto_pwhash_ALG_DEFAULT
    );

    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    randombytes_buf(nonce, sizeof nonce);

    std::vector<unsigned char> cipher(
        crypto_secretbox_MACBYTES + json.size()
    );

    crypto_secretbox_easy(
        cipher.data(),
        (const unsigned char*)json.data(),
        json.size(),
        nonce,
        key
    );

    std::string out;
    out.append((char*)salt, sizeof salt);
    out.append((char*)nonce, sizeof nonce);
    out.append((char*)cipher.data(), cipher.size());

    sodium_memzero(key, sizeof key);
    return out;
}


std::string VaultCrypto::decryptJSON(
    const std::string& data,
    const std::string& masterPassword
) {
    size_t offset = 0;

    unsigned char salt[crypto_pwhash_SALTBYTES];
    memcpy(salt, data.data(), sizeof salt);
    offset += sizeof salt;

    unsigned char key[crypto_secretbox_KEYBYTES];
    crypto_pwhash(
        key, sizeof key,
        masterPassword.c_str(),
        masterPassword.size(),
        salt,
        crypto_pwhash_OPSLIMIT_MODERATE,
        crypto_pwhash_MEMLIMIT_MODERATE,
        crypto_pwhash_ALG_DEFAULT
    );

    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    memcpy(nonce, data.data() + offset, sizeof nonce);
    offset += sizeof nonce;

    size_t cipherLen = data.size() - offset;
    std::vector<unsigned char> plain(
        cipherLen - crypto_secretbox_MACBYTES
    );

    if (crypto_secretbox_open_easy(
        plain.data(),
        (unsigned char*)data.data() + offset,
        cipherLen,
        nonce,
        key
    ) != 0) {
        throw std::runtime_error("Wrong password");
    }

    sodium_memzero(key, sizeof key);
    return std::string((char*)plain.data(), plain.size());
}

