#pragma once

#include <ll/api/mod/NativeMod.h>
#include <nlohmann/json.hpp>
#include <string>

namespace translator::manager {

class ConfigManager final {
public:
    struct MainConfig {
        int         version                            = 1;
        std::string defaultLocaleCode                  = "en_US";
        bool        replaceLeviLaminaDefaultLocaleCode = true;
    };

    static bool              init(ll::mod::NativeMod& mod);
    static const MainConfig& getConfig();

private:
    static MainConfig config;
};

} // namespace translator::manager