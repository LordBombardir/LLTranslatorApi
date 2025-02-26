#pragma once

#include <string>

namespace translator {

class Utils final {
public:
    static std::string generateRandomHex(int maxSymbolsCount = 16);
};

} // namespace translator