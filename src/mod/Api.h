#pragma once

#include <mc/network/packet/AvailableCommandsPacket.h>
#include <mc/world/actor/player/Player.h>
#include <optional>
#include <string>

#ifndef TRANSLATORAPI_API
#ifdef TRANSLATORAPI_EXPORT
#define TRANSLATOR_API __declspec(dllexport)
#else
#define TRANSLATOR_API __declspec(dllimport)
#endif
#endif

extern "C++" {

namespace translator::api {

TRANSLATOR_API void setTranslationForCommandDescription(
    const std::string& commandName,
    const std::string& description,
    const std::string& localeCode
);

TRANSLATOR_API void
removeTranslationForCommandDescription(const std::string& commandName, const std::string& localeCode);

TRANSLATOR_API std::optional<std::string>
               getTranslationForCommandDescription(const std::string& commandName, const std::string& localeCode);

TRANSLATOR_API void sendPlayerAvailableCommandsPacket(const Player& player);

} // namespace translator::api
}