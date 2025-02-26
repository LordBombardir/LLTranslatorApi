#include "Hooks.h"
#include "MainManager.h"
#include <ll/api/memory/Hook.h>
#include <mc/network/ServerNetworkHandler.h>
#include <mc/network/packet/SetLocalPlayerAsInitializedPacket.h>
#include <mc/server/ServerPlayer.h>

namespace translator {

LL_TYPE_INSTANCE_HOOK(
    PlayerJoinHook,
    HookPriority::Normal,
    ServerNetworkHandler,
    &ServerNetworkHandler::$handle,
    void,
    const NetworkIdentifier&                 identifier,
    const SetLocalPlayerAsInitializedPacket& packet
) {
    if (ServerPlayer* player = thisFor<NetEventCallback>()->_getServerPlayer(identifier, packet.mClientSubId); player) {
        MainManager::getAvailableCommandsPacket(*player).sendToClient(identifier, packet.mClientSubId);
    }

    origin(identifier, packet);
}

void Hooks::setupHooks() { PlayerJoinHook::hook(); }

} // namespace translator