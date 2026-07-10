#include "Hooks.h"
#include "../placeholders/PlaceholdersManager.h"

#include <ll/api/i18n/I18n.h>
#include <ll/api/memory/Hook.h>
#include <mc/network/NetworkIdentifierWithSubId.h>
#include <mc/network/NetworkSystem.h>

namespace placeholder {

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
    NetworkSystemSendToMultipleHook::hook();
    NetworkSystemSendHook::hook();
}

} // namespace placeholder