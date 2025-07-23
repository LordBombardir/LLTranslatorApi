#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace translator {

class Utils final {
public:
    static std::string
    strReplace(std::string_view originalStr, std::string_view whatNeedToReplace, std::string_view whatForReplace);

    static std::vector<std::string_view::const_iterator>
    findAllOccurrences(std::string_view haystack, std::string_view needle);
};

} // namespace translator