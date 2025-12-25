#include <vector>
#include <string>
#include <cstring>
#include <iterator>
#include <stdexcept>
#include <sodium.h>
#include "VaultCrypto.h"
#include <fstream>


std::string loadEncrypedFile() {
    std::ifstream file("vault.dat", std::ios::binary);
    return std::string(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );
}

void saveEncrypedFile(const std::string& data) {
    std::ofstream file("vault.dat", std::ios::binary);
    file.write(data.data(), data.size());

}