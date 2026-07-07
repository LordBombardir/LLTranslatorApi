#include "Hooks.h"
#include "../config/ConfigManager.h"
#include "../config/types/Config.h"
#include "../placeholders/PlaceholdersManager.h"

#include <ll/api/i18n/I18n.h>
#include <ll/api/memory/Hook.h>
#include <mc/network/NetworkIdentifierWithSubId.h>
#include <mc/network/NetworkSystem.h>

namespace placeholder {

LL_STATIC_HOOK(
    LeviLaminaDefaultLocaleCodeHook,
    HookPriority::Normal,
    &ll::i18n::getDefaultLocaleCode,
    std::string_view
) {
    return ConfigManager::getConfig().defaultLocaleCode;
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
    PlaceholdersManager::SendScopeGuard guard;
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
    PlaceholdersManager::SendScopeGuard guard;
    return origin(id, PlaceholdersManager::processPacket(id, originalPacket), recipientSubId);
}

void Hooks::setup() {
    if (ConfigManager::getConfig().replaceLeviLaminaDefaultLocaleCode) {
        LeviLaminaDefaultLocaleCodeHook::hook();
    }

    NetworkSystemSendToMultipleHook::hook();
    NetworkSystemSendHook::hook();
}

} // namespace placeholder