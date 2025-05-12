#include "Main.h"
#include "hooks/Hooks.h"
#include <ll/api/mod/RegisterHelper.h>

namespace translator {

Main& Main::getInstance() {
    static Main instance;
    return instance;
}

bool Main::load() {
    hooks::Hooks::setupHooks();
    return true;
}

bool Main::enable() { return true; }

bool Main::disable() { return true; }

} // namespace translator

LL_REGISTER_MOD(translator::Main, translator::Main::getInstance());
