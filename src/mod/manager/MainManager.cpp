#include "MainManager.h"
#include "config/ConfigManager.h"
#include "placeholders/PlaceholdersManager.h"

namespace translator::manager {

static constexpr short timeRemained = 60;

std::unordered_map<std::string, std::unordered_map<std::string, std::string>> MainManager::placeholders = {};

std::unordered_map<std::string, MainManager::TemporaryPlaceholder> MainManager::temporaryPlaceholders = {};

std::mutex MainManager::temporaryPlaceholdersMutex;

bool MainManager::initManagers(ll::mod::NativeMod& mod) { return ConfigManager::init(mod); }

void MainManager::disposeManagers() {
    PlaceholdersManager::cleanPacketsCache(true);
    cleanTemporaryPlaceholders(true);
}

void MainManager::cleanTemporaryPlaceholders(bool forced) {
    std::lock_guard<std::mutex> lock(temporaryPlaceholdersMutex);

    for (auto it = temporaryPlaceholders.begin(); it != temporaryPlaceholders.end();) {
        if (--it->second.secondsToCleanRemain <= 0 || forced) {
            it = temporaryPlaceholders.erase(it);
        }
    }
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
    auto it = placeholders.find(localeCode);
    if (it == placeholders.end()) {
        return {};
    }

    return it->second;
}

void MainManager::setTemporaryPlaceholder(const std::string& placeholder, const std::string& replaceFor) {
    std::lock_guard<std::mutex> lock(temporaryPlaceholdersMutex);

    MainManager::TemporaryPlaceholder temporaryPlaceholder;
    temporaryPlaceholder.secondsToCleanRemain = timeRemained;
    temporaryPlaceholder.replaceFor           = replaceFor;

    temporaryPlaceholders[placeholder] = temporaryPlaceholder;
}

std::optional<std::string> MainManager::getTemporaryPlaceholder(const std::string& placeholder) {
    std::lock_guard<std::mutex> lock(temporaryPlaceholdersMutex);

    auto it = temporaryPlaceholders.find(placeholder);
    if (it == temporaryPlaceholders.end()) {
        return std::nullopt;
    }

    it->second.secondsToCleanRemain = timeRemained;
    return it->second.replaceFor;
}

std::unordered_map<std::string, std::string> MainManager::getTemporaryPlaceholders() {
    std::lock_guard<std::mutex> lock(temporaryPlaceholdersMutex);

    std::unordered_map<std::string, std::string> result;
    for (auto& [placeholder, temporaryPlaceholder] : temporaryPlaceholders) {
        result[placeholder] = temporaryPlaceholder.replaceFor;
    }

    return result;
}

} // namespace translator::manager