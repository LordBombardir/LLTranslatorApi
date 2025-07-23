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

    auto matches = findAllOccurrences(originalStr, whatNeedToReplace);

    auto it = originalStr.begin();
    for (auto match : matches) {
        result.append(it, match);
        result.append(whatForReplace);

        it = match + whatNeedToReplace.size();
    }

    result.append(it, originalStr.end());
    return result;
}

std::vector<std::string_view::const_iterator>
Utils::findAllOccurrences(std::string_view haystack, std::string_view needle) {
    std::vector<std::string_view::const_iterator> matches;
    if (needle.empty()) {
        return matches;
    }

    auto searcher = std::boyer_moore_searcher(needle.begin(), needle.end());

    auto begin = haystack.begin();
    auto end   = haystack.end();
    auto it    = begin;

    while (it != end) {
        auto match = std::search(it, end, searcher);
        if (match == end) {
            break;
        }

        matches.push_back(match);
        it = match + needle.size();
    }

    return matches;
}

} // namespace translator