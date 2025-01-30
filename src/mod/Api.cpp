#include "LLTranslatorApi.h"
#include "MainManager.h"

void setTranslationForCommandDescription(
    const std::string& commandName,
    const std::string& description,
    const std::string& localeCode
) {
    translator::MainManager::setTranslationForCommandDescription(commandName, description, localeCode);
}

void removeTranslationForCommandDescription(const std::string& commandName, const std::string& localeCode) {
    translator::MainManager::removeTranslationForCommandDescription(commandName, localeCode);
}

std::optional<std::string>
getTranslationForCommandDescription(const std::string& commandName, const std::string& localeCode) {
    return translator::MainManager::getTranslationForCommandDescription(commandName, localeCode);
}

AvailableCommandsPacket getAvailableCommandsPacket(Player& player) {
    return translator::MainManager::getAvailableCommandsPacket(player);
}