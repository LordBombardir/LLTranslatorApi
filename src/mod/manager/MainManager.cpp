#include "MainManager.h"
#include "config/ConfigManager.h"
#include "placeholders/PlaceholdersManager.h"

namespace translator::manager {

std::unordered_map<std::string, std::unordered_map<std::string, std::string>> MainManager::placeholders = {};

std::unordered_map<std::string, std::string> MainManager::temporaryPlaceholders = {};

bool MainManager::initManagers(ll::mod::NativeMod& mod) { return ConfigManager::init(mod); }

void MainManager::disposeManagers() { PlaceholdersManager::cleanPacketsCache(); }

void MainManager::cleanTemporaryPlaceholders() {
    temporaryPlaceholders.clear();
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

const std::unordered_map<std::string, std::string>* MainManager::getPlaceholders(const std::string& localeCode) {
    auto it = placeholders.find(localeCode);
    if (it == placeholders.end()) {
        return nullptr;
    }

    return &it->second;
}

void MainManager::setTemporaryPlaceholder(const std::string& placeholder, const std::string& replaceFor) {
    temporaryPlaceholders[placeholder] = replaceFor;
}

std::optional<std::string> MainManager::getTemporaryPlaceholder(const std::string& placeholder) {
    auto it = temporaryPlaceholders.find(placeholder);
    if (it == temporaryPlaceholders.end()) {
        return std::nullopt;
    }

    return it->second;
}

const std::unordered_map<std::string, std::string>& MainManager::getTemporaryPlaceholders() {
    return temporaryPlaceholders;
}

} // namespace translator::manager