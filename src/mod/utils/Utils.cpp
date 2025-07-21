#include "Utils.h"
#include "../Constants.h"
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

std::string
Utils::strReplace(std::string_view originalStr, const std::unordered_map<std::string, std::string>& replacements) {
    std::string result(originalStr);

    do {
        for (const auto& [whatNeedToReplace, whatForReplace] : replacements) {
            std::string updated = Utils::strReplace(result, whatNeedToReplace, whatForReplace);
        }
    } while (result.contains(PREFIX_SCOPE)); // todo: изменить PREFIX_SCOPE на секретно-генерируемое уникальное значение.

    return result;
}

} // namespace translator