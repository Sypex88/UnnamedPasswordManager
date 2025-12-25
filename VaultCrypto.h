#ifndef UNTITLED2_VAULTCRYPTO_H
#define UNTITLED2_VAULTCRYPTO_H
#pragma once
#include <string>

class VaultCrypto {
public:
    static std::string encryptJSON(
        const std::string& json,
        const std::string& masterPassword
    );

    static std::string decryptJSON(
        const std::string& encryptedData,
        const std::string& masterPassword
    );
};


#endif //UNTITLED2_VAULTCRYPTO_H