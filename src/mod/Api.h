#pragma once

#include <optional>
#include <string>
#include <unordered_map>

#ifndef PLACEHOLDER_API
#ifdef PLACEHOLDERAPI_EXPORT
#define PLACEHOLDER_API __declspec(dllexport)
#else
#define PLACEHOLDER_API __declspec(dllimport)
#endif
#endif

extern "C++" {

namespace placeholder::api {

// DO NOT use for commands!
PLACEHOLDER_API std::string generatePlaceholder(std::string_view placeholder);
PLACEHOLDER_API std::string generateTemporaryPlaceholder();

// Use generatePlaceholder!
PLACEHOLDER_API void
setPlaceholder(const std::string& placeholder, const std::string& replaceFor, const std::string& localeCode);
PLACEHOLDER_API std::optional<std::string> getPlaceholder(const std::string& placeholder, const std::string& localeCode);
PLACEHOLDER_API void removePlaceholder(const std::string& placeholder, const std::string& localeCode);
PLACEHOLDER_API std::unordered_map<std::string, std::string> getPlaceholders(const std::string& localeCode);

// Use generateTemporaryPlaceholder!
PLACEHOLDER_API void
setTemporaryPlaceholder(const std::string& placeholder, const std::string& replaceFor, const std::string& localeCode);
PLACEHOLDER_API std::optional<std::string>
               getTemporaryPlaceholder(const std::string& placeholder, const std::string& localeCode);
PLACEHOLDER_API std::unordered_map<std::string, std::string> getTemporaryPlaceholders(const std::string& localeCode);

} // namespace placeholder::api
}