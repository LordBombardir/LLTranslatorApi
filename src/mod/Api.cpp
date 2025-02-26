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

std::string addFunctionToProcessingPacket(const std::function<void(AvailableCommandsPacket&)>& function) {
    return addFunctionToProcessingPacket(false, false, std::nullopt, function);
}

std::string addFunctionToProcessingPacket(bool isFirst, const std::function<void(AvailableCommandsPacket&)>& function) {
    return addFunctionToProcessingPacket(isFirst, false, std::nullopt, function);
}

std::string addFunctionToProcessingPacket(
    bool                                                 isFirst,
    bool                                                 isOnce,
    const std::function<void(AvailableCommandsPacket&)>& function
) {
    return addFunctionToProcessingPacket(isFirst, isOnce, std::nullopt, function);
}

std::string addFunctionToProcessingPacket(
    bool                                                 isFirst,
    bool                                                 isOnce,
    const std::optional<std::string>&                    forPlayer,
    const std::function<void(AvailableCommandsPacket&)>& function
) {
    return translator::MainManager::addFunctionProcessingPacket(isFirst, isOnce, forPlayer, function);
}

void removeFunctionToProcessingPacket(const std::string& id) {
    translator::MainManager::removeFunctionProcessingPacket(id);
}
