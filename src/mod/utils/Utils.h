#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

namespace translator {

class Utils final {
public:
    static std::string
    strReplace(std::string_view originalStr, std::string_view whatNeedToReplace, std::string_view whatForReplace);
    static std::string
    strReplace(std::string_view originalStr, const std::unordered_map<std::string, std::string>& replacements);
};

} // namespace translator