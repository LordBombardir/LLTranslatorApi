#include "Api.h"
#include "Constants.h"
#include "manager/MainManager.h"
#include <mc/platform/UUID.h>

namespace translator::api {

constexpr std::string generatePlaceholder(std::string_view placeholder) {
    return std::string(PREFIX_SCOPE) + std::string(placeholder);
}

std::string generateTemporaryPlaceholder() {
    const auto& uuid = mce::UUID::random();
    return std::string(PREFIX_SCOPE) + std::to_string(uuid.a + uuid.b);
}

void setPlaceholder(const std::string& placeholder, const std::string& replaceFor, const std::string& localeCode) {
    if (replaceFor.contains(PREFIX_SCOPE)) {
        return;
    }

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
    if (replaceFor.contains(PREFIX_SCOPE)) {
        return;
    }

    manager::MainManager::setTemporaryPlaceholder(placeholder, replaceFor, localeCode);
}

std::optional<std::string> getTemporaryPlaceholder(const std::string& placeholder, const std::string& localeCode) {
    return manager::MainManager::getTemporaryPlaceholder(placeholder, localeCode);
}

std::unordered_map<std::string, std::string> getTemporaryPlaceholders(const std::string& localeCode) {
    return manager::MainManager::getTemporaryPlaceholders(localeCode);
}

} // namespace translator::api
