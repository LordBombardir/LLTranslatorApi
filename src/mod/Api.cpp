#include "Api.h"
#include "Constants.h"
#include "manager/MainManager.h"

namespace translator::api {

constexpr std::string generatePlaceholder(std::string_view placeholder) {
    return std::string(PREFIX_SCOPE) + std::string(placeholder);
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

} // namespace translator::api
