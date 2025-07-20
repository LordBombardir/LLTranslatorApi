#pragma once

#include <string>
#include <string_view>

namespace translator {

class Utils final {
public:
    static std::string strReplace(std::string_view originalStr, std::string_view whatNeedToReplace, std::string_view whatForReplace);
};

} // namespace translator