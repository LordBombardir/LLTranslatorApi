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

    static void cleanTemporaryPlaceholders();

    static void
    setPlaceholder(const std::string& placeholder, const std::string& replaceFor, const std::string& localeCode);

    static std::optional<std::string> getPlaceholder(const std::string& placeholder, const std::string& localeCode);

    static void removePlaceholder(const std::string& placeholder, const std::string& localeCode);

    static const std::unordered_map<std::string, std::string>* getPlaceholders(const std::string& localeCode);

    static void setTemporaryPlaceholder(const std::string& placeholder, const std::string& replaceFor);

    static std::optional<std::string> getTemporaryPlaceholder(const std::string& placeholder);

    static const std::unordered_map<std::string, std::string>& getTemporaryPlaceholders();

private:
    static std::unordered_map<std::string, std::unordered_map<std::string, std::string>> placeholders;

    static std::unordered_map<std::string, std::string> temporaryPlaceholders;
};

} // namespace translator::manager