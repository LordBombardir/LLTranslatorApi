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

/**
 * @brief Generates a unique placeholder key from a given identifier.
 * @note DO NOT use the resulting key for command names as command names are immutable.
 * @param placeholder The unique key identifier/name.
 * @return The formatted placeholder key (e.g., "prefix:hash").
 */
PLACEHOLDER_API std::string generatePlaceholder(std::string_view placeholder);

/**
 * @brief Generates a random temporary placeholder key.
 * @return The formatted temporary placeholder key (e.g., "prefix:random_hash").
 */
PLACEHOLDER_API std::string generateTemporaryPlaceholder();

/**
 * @brief Registers or updates a persistent placeholder translation.
 * @note Use generatePlaceholder() to get the unique key to pass here.
 * @param placeholder The generated placeholder key.
 * @param replaceFor The translation text to substitute the placeholder with.
 * @param localeCode The locale identifier (e.g., "en_US").
 */
PLACEHOLDER_API void
setPlaceholder(const std::string& placeholder, const std::string& replaceFor, const std::string& localeCode);

/**
 * @brief Retrieves the translation text for a persistent placeholder.
 * @param placeholder The generated placeholder key.
 * @param localeCode The locale identifier (e.g., "en_US").
 * @return The translation text, or std::nullopt if not registered.
 */
PLACEHOLDER_API std::optional<std::string> getPlaceholder(const std::string& placeholder, const std::string& localeCode);

/**
 * @brief Removes a persistent placeholder translation.
 * @param placeholder The generated placeholder key.
 * @param localeCode The locale identifier (e.g., "en_US").
 */
PLACEHOLDER_API void removePlaceholder(const std::string& placeholder, const std::string& localeCode);

/**
 * @brief Retrieves all registered persistent placeholders for a locale.
 * @param localeCode The locale identifier (e.g., "en_US").
 * @return A map of placeholder keys to translation values.
 */
PLACEHOLDER_API std::unordered_map<std::string, std::string> getPlaceholders(const std::string& localeCode);

/**
 * @brief Registers or updates a temporary placeholder translation.
 * @note Temporary placeholders are automatically cleaned up after a period of inactivity.
 * @note Use generateTemporaryPlaceholder() to get the unique key to pass here.
 * @param placeholder The generated temporary placeholder key.
 * @param replaceFor The translation text to substitute the placeholder with.
 * @param localeCode The locale identifier (e.g., "en_US").
 */
PLACEHOLDER_API void
setTemporaryPlaceholder(const std::string& placeholder, const std::string& replaceFor, const std::string& localeCode);

/**
 * @brief Retrieves the translation text for a temporary placeholder and resets its activity timer.
 * @param placeholder The generated temporary placeholder key.
 * @param localeCode The locale identifier (e.g., "en_US").
 * @return The translation text, or std::nullopt if not registered or expired.
 */
PLACEHOLDER_API std::optional<std::string>
getTemporaryPlaceholder(const std::string& placeholder, const std::string& localeCode);

/**
 * @brief Retrieves all registered temporary placeholders for a locale.
 * @param localeCode The locale identifier (e.g., "en_US").
 * @return A map of temporary placeholder keys to translation values.
 */
PLACEHOLDER_API std::unordered_map<std::string, std::string> getTemporaryPlaceholders(const std::string& localeCode);

} // namespace placeholder::api
}