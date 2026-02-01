#include "Hooks.h"
#include "../manager/config/ConfigManager.h"
#include "../manager/placeholders/PlaceholdersManager.h"
#include <ll/api/i18n/I18n.h>
#include <ll/api/memory/Hook.h>
#include <mc/network/NetworkIdentifierWithSubId.h>
#include <mc/network/NetworkSystem.h>

namespace placeholder::hooks {

LL_STATIC_HOOK(
    LeviLaminaDefaultLocaleCodeHook,
    HookPriority::Normal,
    &ll::i18n::getDefaultLocaleCode,
    std::string_view
) {
    return manager::ConfigManager::getConfig().defaultLocaleCode;
}

LL_TYPE_INSTANCE_HOOK(
    NetworkSystemSendToMultipleHook,
    HookPriority::Normal,
    NetworkSystem,
    &NetworkSystem::sendToMultiple,
    void,
    const std::vector<NetworkIdentifierWithSubId>& ids,
    const Packet&                                  packet
) {
    for (const NetworkIdentifierWithSubId& id : ids) {
        thisFor<NetworkSystem>()->send(id.id, packet, id.subClientId);
    }
}

LL_TYPE_INSTANCE_HOOK(
    NetworkSystemSendHook,
    HookPriority::Normal,
    NetworkSystem,
    &NetworkSystem::send,
    void,
    const NetworkIdentifier& id,
    const Packet&            originalPacket,
    SubClientId              recipientSubId
) {
    return origin(id, manager::PlaceholdersManager::processPacket(id, originalPacket), recipientSubId);
}

void setupHooks() {
    if (manager::ConfigManager::getConfig().replaceLeviLaminaDefaultLocaleCode) {
        LeviLaminaDefaultLocaleCodeHook::hook();
    }

    NetworkSystemSendToMultipleHook::hook();
    NetworkSystemSendHook::hook();
}

} // namespace placeholder::hooks