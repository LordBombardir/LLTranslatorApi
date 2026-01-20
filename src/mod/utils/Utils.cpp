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

    size_t lastPos = 0;
    for (size_t matchPos : matches) {
        result.append(originalStr.substr(lastPos, matchPos - lastPos));
        result.append(whatForReplace);

        lastPos = matchPos + whatNeedToReplace.size();
    }

    result.append(originalStr.substr(lastPos));
    return result;
}

std::vector<size_t> Utils::findAllOccurrences(std::string_view haystack, std::string_view needle) {
    std::vector<size_t> matches;
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

        size_t pos = static_cast<size_t>(std::distance(haystack.begin(), match));
        matches.push_back(pos);

        it = match + needle.size();
    }

    return matches;
}

} // namespace translator