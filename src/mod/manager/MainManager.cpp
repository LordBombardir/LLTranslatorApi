#include "MainManager.h"
#include <ll/api/service/Bedrock.h>
#include <mc/server/commands/CommandRegistry.h>

AvailableCommandsPacket::EnumData::EnumData(const EnumData&)                                     = default;
AvailableCommandsPacket::SoftEnumData::SoftEnumData(const SoftEnumData&)                         = default;
AvailableCommandsPacket::ConstrainedValueData::ConstrainedValueData(const ConstrainedValueData&) = default;
AvailableCommandsPacket::ParamData::ParamData(const ParamData&)                                  = default;
AvailableCommandsPacket::OverloadData::OverloadData(const OverloadData&)                         = default;
AvailableCommandsPacket::CommandData::CommandData(const CommandData&)                            = default;

namespace translator::manager {

std::unordered_map<std::string, std::unordered_map<std::string, std::string>>
    MainManager::translationCommandDescription = {};

void MainManager::setTranslationForCommandDescription(
    const std::string& commandName,
    const std::string& description,
    const std::string& localeCode
) {
    translationCommandDescription[commandName][localeCode] = description;
}

void MainManager::removeTranslationForCommandDescription(
    const std::string& commandName,
    const std::string& localeCode
) {
    auto firstIt = translationCommandDescription.find(commandName);
    if (firstIt == translationCommandDescription.end()) {
        return;
    }

    auto secondIt = firstIt->second.find(localeCode);
    if (secondIt == firstIt->second.end()) {
        return;
    }

    firstIt->second.erase(secondIt);
}

std::optional<std::string>
MainManager::getTranslationForCommandDescription(const std::string& commandName, const std::string& localeCode) {
    auto firstIt = translationCommandDescription.find(commandName);
    if (firstIt == translationCommandDescription.end()) {
        return std::nullopt;
    }

    auto secondIt = firstIt->second.find(localeCode);
    if (secondIt == firstIt->second.end()) {
        return std::nullopt;
    }

    return secondIt->second;
}

AvailableCommandsPacket MainManager::getAvailableCommandsPacket(const Player& player) {
    AvailableCommandsPacket packet = ll::service::getCommandRegistry()->serializeAvailableCommands();
    std::vector<AvailableCommandsPacket::CommandData>& commands = packet.mCommands.get();

    for (AvailableCommandsPacket::CommandData& command : commands) {
        std::optional<std::string> translationForCommandDescription =
            getTranslationForCommandDescription(command.name.get(), player.getLocaleCode());
        if (translationForCommandDescription.has_value()) {
            command.description = translationForCommandDescription.value();
        }
    }

    return packet;
}

} // namespace translator::manager