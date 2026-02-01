#include "ConfigManager.h"
#include <ll/api/Config.h>

namespace placeholder::manager {

ConfigManager::MainConfig ConfigManager::config;

bool ConfigManager::init(ll::mod::NativeMod& mod) {
    std::filesystem::path pathToConfig = mod.getDataDir() / "config.json";

    try {
        return ll::config::loadConfig(config, pathToConfig);
    } catch (...) {}

    try {
        return ll::config::saveConfig(config, pathToConfig);
    } catch (...) {
        return false;
    }
}

const ConfigManager::MainConfig& ConfigManager::getConfig() { return config; }

} // namespace placeholder::manager