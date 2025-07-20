#include "Api.h"
#include "Constants.h"
#include "manager/MainManager.h"
#include <mc/deps/crypto/hash/Hash.h>
#include <mc/platform/UUID.h>

namespace translator::api {

constexpr std::string generatePlaceholder(std::string_view placeholder) {
    return std::string(PREFIX_SCOPE) + std::string(placeholder);
}

constexpr std::string generateTemporaryPlaceholder(const std::string& replaceFor) {
    const auto& uuid = Crypto::Hash::generateUUID(replaceFor.data(), replaceFor.size(), Crypto::Hash::HashType::Md5);
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

const std::unordered_map<std::string, std::string>* getPlaceholders(const std::string& localeCode) {
    return manager::MainManager::getPlaceholders(localeCode);
}

std::optional<std::string> getTemporaryPlaceholder(const std::string& placeholder) {
    return manager::MainManager::getTemporaryPlaceholder(placeholder);
}

const std::unordered_map<std::string, std::string>& getTemporaryPlaceholders() {
    return manager::MainManager::getTemporaryPlaceholders();
}

} // namespace translator::api
