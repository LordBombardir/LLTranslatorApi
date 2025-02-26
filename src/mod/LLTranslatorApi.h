#pragma once

#include <functional>
#include <mc/network/packet/AvailableCommandsPacket.h>
#include <mc/world/actor/player/Player.h>
#include <optional>
#include <string>

#ifdef TRANSLATORAPI_EXPORT
#define TRANSLATOR_API __declspec(dllexport)
#else
#define TRANSLATOR_API __declspec(dllimport)
#endif

extern "C++" {

TRANSLATOR_API void setTranslationForCommandDescription(
    const std::string& commandName,
    const std::string& description,
    const std::string& localeCode
);

TRANSLATOR_API void
removeTranslationForCommandDescription(const std::string& commandName, const std::string& localeCode);

TRANSLATOR_API std::optional<std::string>
               getTranslationForCommandDescription(const std::string& commandName, const std::string& localeCode);

TRANSLATOR_API std::string addFunctionToProcessingPacket(const std::function<void(AvailableCommandsPacket&)>& function);
TRANSLATOR_API             std::string
addFunctionToProcessingPacket(bool isFirst, const std::function<void(AvailableCommandsPacket&)>& function);
TRANSLATOR_API std::string
addFunctionToProcessingPacket(bool isFirst, bool isOnce, const std::function<void(AvailableCommandsPacket&)>& function);
TRANSLATOR_API std::string addFunctionToProcessingPacket(
    bool                                                 isFirst,
    bool                                                 isOnce,
    const std::optional<std::string>&                    forPlayer,
    const std::function<void(AvailableCommandsPacket&)>& function
);

TRANSLATOR_API void removeFunctionToProcessingPacket(const std::string& id);
}