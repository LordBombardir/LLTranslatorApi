#include "LLTranslatorApi.h"
#include "manager/MainManager.h"

namespace translator::api {

void setTranslationForCommandDescription(
    const std::string& commandName,
    const std::string& description,
    const std::string& localeCode
) {
    manager::MainManager::setTranslationForCommandDescription(commandName, description, localeCode);
}

void removeTranslationForCommandDescription(const std::string& commandName, const std::string& localeCode) {
    manager::MainManager::removeTranslationForCommandDescription(commandName, localeCode);
}

std::optional<std::string>
getTranslationForCommandDescription(const std::string& commandName, const std::string& localeCode) {
    return manager::MainManager::getTranslationForCommandDescription(commandName, localeCode);
}

AvailableCommandsPacket getAvailableCommandsPacket(const Player& player) {
    return manager::MainManager::getAvailableCommandsPacket(player);
}

void sendPlayerAvailableCommandsPacket(const Player& player) {
    manager::MainManager::getAvailableCommandsPacket(player).sendToClient(
        player.getNetworkIdentifier(),
        player.getClientSubId()
    );
}

} // namespace translator::api
