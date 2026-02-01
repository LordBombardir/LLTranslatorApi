#include "Api.h"
#include "manager/MainManager.h"
#include <ll/api/utils/HashUtils.h>
#include <mc/platform/UUID.h>

namespace placeholder::api {

std::string hash(std::string_view string) {
    std::ostringstream oss;

    oss << std::hex << std::setw(16) << std::setfill('0') << ll::hash_utils::doHash(string);
    return oss.str();
}

std::string generatePlaceholder(std::string_view placeholder) {
    return std::format("{}:{}", manager::MainManager::getPrefixScope(), hash(placeholder));
}

std::string generateTemporaryPlaceholder() {
    return std::format("{}:{}", manager::MainManager::getPrefixScope(), hash(mce::UUID::random().asString()));
}

void setPlaceholder(const std::string& placeholder, const std::string& replaceFor, const std::string& localeCode) {
    manager::MainManager::setPlaceholder(placeholder, replaceFor, localeCode);
}

std::optional<std::string> getPlaceholder(const std::string& placeholder, const std::string& localeCode) {
    return manager::MainManager::getPlaceholder(placeholder, localeCode);
}

void removePlaceholder(const std::string& placeholder, const std::string& localeCode) {
    manager::MainManager::removePlaceholder(placeholder, localeCode);
}

std::unordered_map<std::string, std::string> getPlaceholders(const std::string& localeCode) {
    return manager::MainManager::getPlaceholders(localeCode);
}

void setTemporaryPlaceholder(
    const std::string& placeholder,
    const std::string& replaceFor,
    const std::string& localeCode
) {
    manager::MainManager::setTemporaryPlaceholder(placeholder, replaceFor, localeCode);
}

std::optional<std::string> getTemporaryPlaceholder(const std::string& placeholder, const std::string& localeCode) {
    return manager::MainManager::getTemporaryPlaceholder(placeholder, localeCode);
}

std::unordered_map<std::string, std::string> getTemporaryPlaceholders(const std::string& localeCode) {
    return manager::MainManager::getTemporaryPlaceholders(localeCode);
}

} // namespace placeholder::api
