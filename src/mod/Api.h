#pragma once

#include <optional>
#include <string>
#include <unordered_map>

#ifndef TRANSLATORAPI_API
#ifdef TRANSLATORAPI_EXPORT
#define TRANSLATOR_API __declspec(dllexport)
#else
#define TRANSLATOR_API __declspec(dllimport)
#endif
#endif

extern "C++" {

namespace translator::api {

// DO NOT use for commands!
TRANSLATOR_API constexpr std::string generatePlaceholder(std::string_view placeholder);

// Use generatePlaceholder!
TRANSLATOR_API void
setPlaceholder(const std::string& placeholder, const std::string& replaceFor, const std::string& localeCode);

TRANSLATOR_API std::optional<std::string> getPlaceholder(const std::string& placeholder, const std::string& localeCode);

TRANSLATOR_API void removePlaceholder(const std::string& placeholder, const std::string& localeCode);

TRANSLATOR_API std::unordered_map<std::string, std::string> getPlaceholders(const std::string& localeCode);

} // namespace translator::api
}