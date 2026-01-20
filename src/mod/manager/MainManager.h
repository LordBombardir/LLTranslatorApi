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
    static std::recursive_mutex temporaryPlaceholdersMutex;

    static std::string generateKey(size_t length);

    template <typename MapT, typename ExtractValue>
    static auto mergeLocaleMaps(
        const MapT&        storage,
        const std::string& defaultLocale,
        const std::string& localeCode,
        ExtractValue&&     extract
    ) -> std::unordered_map<std::string, std::string> {
        std::unordered_map<std::string, std::string> result;

        if (auto it = storage.find(defaultLocale); it != storage.end()) {
            result.reserve(it->second.size());
            for (const auto& [key, val] : it->second) {
                result[key] = extract(val);
            }
        }

        if (localeCode != defaultLocale) {
            if (auto it = storage.find(localeCode); it != storage.end()) {
                result.reserve(result.size() + it->second.size());
                for (const auto& [key, val] : it->second) {
                    result[key] = extract(val);
                }
            }
        }

        return result;
    }
};

} // namespace translator::manager