#include "Utils.h"
#include <algorithm>
#include <functional>

namespace translator {

std::string
Utils::strReplace(std::string_view originalStr, std::string_view whatNeedToReplace, std::string_view whatForReplace) {
    if (whatNeedToReplace.empty()) {
        return std::string(originalStr);
    }

    std::string result;
    result.reserve(originalStr.size());

    auto searcher = std::boyer_moore_searcher(whatNeedToReplace.begin(), whatNeedToReplace.end());

    auto begin = originalStr.begin();
    auto end   = originalStr.end();
    auto it    = begin;

    while (it != end) {
        auto match = std::search(it, end, searcher);
        result.append(it, match);

        if (match == end) {
            break;
        }

        result.append(whatForReplace);
        it = match + whatNeedToReplace.size();
    }

    return result;
}

} // namespace translator