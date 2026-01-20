#include "Main.h"
#include "hooks/Hooks.h"
#include "manager/MainManager.h"
#include "tasks/CacheCleanerTask.h"
#include <ll/api/mod/RegisterHelper.h>

namespace translator {

Main& Main::getInstance() {
    static Main instance;
    return instance;
}

bool Main::load() {
    getSelf().getLogger().info("The mod is loading...");

    if (!manager::MainManager::initManagers(getSelf())) {
        getSelf().getLogger().info("Failed to load the mod!");
        return false;
    }

    hooks::setupHooks();

    getSelf().getLogger().info("The mod has been successfully loaded!");
    return true;
}

bool Main::enable() {
    getSelf().getLogger().info("The mod is enabling...");

    if (!tasks::CacheCleanerTask::init()) {
        getSelf().getLogger().info("Failed to enable the mod!");
        return false;
    }

    getSelf().getLogger().info("The mod has been successfully enabled!");

    getSelf().getLogger().info("Author: vk.com/lordbomba");
    return true;
}

bool Main::disable() {
    getSelf().getLogger().info("The mod is disabling...");

    manager::MainManager::disposeManagers();

    getSelf().getLogger().info("The mod has been successfully disabled.");
    return true;
}

} // namespace translator

LL_REGISTER_MOD(translator::Main, translator::Main::getInstance());
