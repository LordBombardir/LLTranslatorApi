#include "Utils.h"
#include <random>

namespace translator {

std::string Utils::generateRandomHex(int maxSymbolsCount) {
    std::random_device                 rd;
    std::mt19937                       rand(rd());
    std::uniform_int_distribution<int> dist(0, 15);

    const std::string allowedChars = "0123456789ABCDEF";
    std::string       hexString;

    for (int i = 0; i < maxSymbolsCount; ++i) {
        hexString += allowedChars[dist(rand)];
    }

    return hexString;
}

} // namespace translator