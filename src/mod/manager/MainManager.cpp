#include "MainManager.h"
#include "config/ConfigManager.h"
#include "placeholders/PlaceholdersManager.h"
#include <ll/api/utils/HashUtils.h>

namespace translator::manager {

static constexpr short timeRemained = 60;

std::unordered_map<std::string, std::unordered_map<std::string, std::string>> MainManager::placeholders = {};

std::unordered_map<std::string, std::unordered_map<std::string, MainManager::TemporaryPlaceholder>>
    MainManager::temporaryPlaceholders = {};

std::recursive_mutex MainManager::temporaryPlaceholdersMutex;

bool MainManager::initManagers(ll::mod::NativeMod& mod) { return ConfigManager::init(mod); }

void MainManager::disposeManagers() {
    PlaceholdersManager::cleanPackets(true);
    cleanTemporaryPlaceholders(true);
}

void MainManager::cleanTemporaryPlaceholders(bool forced) {
    std::lock_guard<std::recursive_mutex> lock(temporaryPlaceholdersMutex);

    for (auto& [localeCode, temporaryPlaceholder] : temporaryPlaceholders) {
        for (auto it = temporaryPlaceholder.begin(); it != temporaryPlaceholder.end();) {
            if (--it->second.secondsToCleanRemain <= 0 || forced) {
                it = temporaryPlaceholder.erase(it);
            }
        }
    }
}

std::string MainManager::getPrefixScope() {
    static std::string prefixScope = generateKey(16);
    return prefixScope;
}

void MainManager::setPlaceholder(
    const std::string& placeholder,
    const std::string& replaceFor,
    const std::string& localeCode
) {
    placeholders[localeCode][placeholder] = replaceFor;
}

std::optional<std::string> MainManager::getPlaceholder(const std::string& placeholder, const std::string& localeCode) {
    auto firstIt = placeholders.find(localeCode);
    if (firstIt == placeholders.end()) {
        if (localeCode == ConfigManager::getConfig().defaultLocaleCode) {
            return std::nullopt;
        }

        return getPlaceholder(placeholder, ConfigManager::getConfig().defaultLocaleCode);
    }

    auto secondIt = firstIt->second.find(placeholder);
    if (secondIt == firstIt->second.end()) {
        return std::nullopt;
    }

    return secondIt->second;
}

void MainManager::removePlaceholder(const std::string& placeholder, const std::string& localeCode) {
    auto firstIt = placeholders.find(localeCode);
    if (firstIt == placeholders.end()) {
        return;
    }

    auto secondIt = firstIt->second.find(placeholder);
    if (secondIt == firstIt->second.end()) {
        return;
    }

    firstIt->second.erase(secondIt);
}

std::unordered_map<std::string, std::string> MainManager::getPlaceholders(const std::string& localeCode) {
    return mergeLocaleMaps(
        placeholders,
        ConfigManager::getConfig().defaultLocaleCode,
        localeCode,
        [](const std::string& v) -> const std::string& { return v; }
    );
}

void MainManager::setTemporaryPlaceholder(
    const std::string& placeholder,
    const std::string& replaceFor,
    const std::string& localeCode
) {
    std::lock_guard<std::recursive_mutex> lock(temporaryPlaceholdersMutex);

    MainManager::TemporaryPlaceholder temporaryPlaceholder;
    temporaryPlaceholder.secondsToCleanRemain = timeRemained;
    temporaryPlaceholder.replaceFor           = replaceFor;

    temporaryPlaceholders[localeCode][placeholder] = temporaryPlaceholder;
}

std::optional<std::string>
MainManager::getTemporaryPlaceholder(const std::string& placeholder, const std::string& localeCode) {
    std::lock_guard<std::recursive_mutex> lock(temporaryPlaceholdersMutex);

    auto firstIt = temporaryPlaceholders.find(localeCode);
    if (firstIt == temporaryPlaceholders.end()) {
        if (localeCode == ConfigManager::getConfig().defaultLocaleCode) {
            return std::nullopt;
        }

        return getTemporaryPlaceholder(placeholder, ConfigManager::getConfig().defaultLocaleCode);
    }

    auto secondIt = firstIt->second.find(placeholder);
    if (secondIt == firstIt->second.end()) {
        return std::nullopt;
    }

    secondIt->second.secondsToCleanRemain = timeRemained;
    return secondIt->second.replaceFor;
}

std::unordered_map<std::string, std::string> MainManager::getTemporaryPlaceholders(const std::string& localeCode) {
    std::lock_guard<std::recursive_mutex> lock(temporaryPlaceholdersMutex);

    return mergeLocaleMaps(
        temporaryPlaceholders,
        ConfigManager::getConfig().defaultLocaleCode,
        localeCode,
        [](const TemporaryPlaceholder& v) -> const std::string& { return v.replaceFor; }
    );
}

std::string MainManager::generateKey(size_t length) {
    static constexpr std::string_view charset = "abcdefghijklmnopqrstuvwxyz"
                                                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                                "0123456789";

    static thread_local std::mt19937                               rng{std::random_device{}()};
    static thread_local std::uniform_int_distribution<std::size_t> dist(0, charset.size() - 1);

    std::string result;
    result.reserve(length);

    for (std::size_t i = 0; i < length; ++i) {
        result += charset[dist(rng)];
    }

    return result;
}

} // namespace translator::manager