#pragma once

#include <functional>
#include <mc/network/packet/AvailableCommandsPacket.h>
#include <mc/world/actor/player/Player.h>
#include <optional>
#include <string>
#include <unordered_map>

namespace translator {

class MainManager final {
public:
    static void setTranslationForCommandDescription(
        const std::string& commandName,
        const std::string& description,
        const std::string& localeCode
    );

    static void removeTranslationForCommandDescription(const std::string& commandName, const std::string& localeCode);

    static std::optional<std::string>
    getTranslationForCommandDescription(const std::string& commandName, const std::string& localeCode);

    static AvailableCommandsPacket getAvailableCommandsPacket(const Player& player);

    static std::string addFunctionProcessingPacket(
        bool                                                 isFirst,
        bool                                                 isOnce,
        const std::optional<std::string>&                    forPlayer,
        const std::function<void(AvailableCommandsPacket&)>& function
    );

    static void removeFunctionProcessingPacket(const std::string& id);

private:
    struct FunctionProcessing {
        bool                                          isFirst;
        bool                                          isOnce;
        std::optional<std::string>                    forPlayer;
        std::function<void(AvailableCommandsPacket&)> function;
    };

    static std::unordered_map<std::string, std::unordered_map<std::string, std::string>> translationCommandDescription;
    static std::unordered_map<std::string, FunctionProcessing>                           functionsForProcessing;
};

} // namespace translator