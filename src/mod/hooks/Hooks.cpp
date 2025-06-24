#include "Hooks.h"
#include "../manager/MainManager.h"
#include <ll/api/memory/Hook.h>
#include <mc/network/ServerNetworkHandler.h>
#include <mc/network/packet/SetLocalPlayerAsInitializedPacket.h>
#include <mc/server/ServerPlayer.h>

namespace translator::hooks {

LL_TYPE_INSTANCE_HOOK(
    PlayerJoinHook,
    HookPriority::Normal,
    ServerNetworkHandler,
    &ServerNetworkHandler::$handle,
    void,
    const NetworkIdentifier&                 identifier,
    const SetLocalPlayerAsInitializedPacket& packet
) {
    if (ServerPlayer* player = thisFor<NetEventCallback>()->_getServerPlayer(identifier, packet.mSenderSubId); player) {
        manager::MainManager::getAvailableCommandsPacket(*player).sendToClient(identifier, player->getClientSubId());
    }

    origin(identifier, packet);
}

void setupHooks() { PlayerJoinHook::hook(); }

} // namespace translator::hooks