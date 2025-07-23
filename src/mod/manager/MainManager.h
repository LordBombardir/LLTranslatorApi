#pragma once

#include <ll/api/mod/NativeMod.h>
#include <optional>
#include <string>
#include <unordered_map>

namespace translator::manager {

class MainManager final {
public:
    static bool initManagers(ll::mod::NativeMod& mod);
    static void disposeManagers();

    static void        cleanTemporaryPlaceholders(bool forced = false);
    static std::string getPrefixScope();

    static void
    setPlaceholder(const std::string& placeholder, const std::string& replaceFor, const std::string& localeCode);

    static std::optional<std::string> getPlaceholder(const std::string& placeholder, const std::string& localeCode);

    static void removePlaceholder(const std::string& placeholder, const std::string& localeCode);

    static std::unordered_map<std::string, std::string> getPlaceholders(const std::string& localeCode);

    static void setTemporaryPlaceholder(
        const std::string& placeholder,
        const std::string& replaceFor,
        const std::string& localeCode
    );

    static std::optional<std::string>
    getTemporaryPlaceholder(const std::string& placeholder, const std::string& localeCode);

    static std::unordered_map<std::string, std::string> getTemporaryPlaceholders(const std::string& localeCode);

private:
    static std::unordered_map<std::string, std::unordered_map<std::string, std::string>> placeholders;

    struct TemporaryPlaceholder {
        short       secondsToCleanRemain;
        std::string replaceFor;
    };

    static std::unordered_map<std::string, std::unordered_map<std::string, TemporaryPlaceholder>> temporaryPlaceholders;
    static std::mutex temporaryPlaceholdersMutex;

    static std::string generateKey(size_t length);
};

} // namespace translator::manager