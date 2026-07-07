#include "Api.h"
#include "MainManager.h"

#include <ll/api/utils/HashUtils.h>
#include <mc/platform/UUID.h>
#include <format>

namespace placeholder::api {

std::string hash(std::string_view string) {
    return std::format("{:016x}", ll::hash_utils::doHash(string));
}

std::string generatePlaceholder(std::string_view placeholder) {
    return std::format("{}:{}", MainManager::getPrefixScope(), hash(placeholder));
}

std::string generateTemporaryPlaceholder() {
    return std::format("{}:{}", MainManager::getPrefixScope(), hash(mce::UUID::random().asString()));
}

void setPlaceholder(const std::string& placeholder, const std::string& replaceFor, const std::string& localeCode) {
    MainManager::setPlaceholder(placeholder, replaceFor, localeCode);
}

std::optional<std::string> getPlaceholder(const std::string& placeholder, const std::string& localeCode) {
    return MainManager::getPlaceholder(placeholder, localeCode);
}

void removePlaceholder(const std::string& placeholder, const std::string& localeCode) {
    MainManager::removePlaceholder(placeholder, localeCode);
}

std::unordered_map<std::string, std::string> getPlaceholders(const std::string& localeCode) {
    return MainManager::getPlaceholders(localeCode);
}

void setTemporaryPlaceholder(
    const std::string& placeholder,
    const std::string& replaceFor,
    const std::string& localeCode
) {
    MainManager::setTemporaryPlaceholder(placeholder, replaceFor, localeCode);
}

std::optional<std::string> getTemporaryPlaceholder(const std::string& placeholder, const std::string& localeCode) {
    return MainManager::getTemporaryPlaceholder(placeholder, localeCode);
}

std::unordered_map<std::string, std::string> getTemporaryPlaceholders(const std::string& localeCode) {
    return MainManager::getTemporaryPlaceholders(localeCode);
}

} // namespace placeholder::api
