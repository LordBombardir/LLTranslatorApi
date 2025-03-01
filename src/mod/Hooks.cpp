#include "Hooks.h"
#include "MainManager.h"
#include <ll/api/memory/Hook.h>
#include <mc/network/ServerNetworkHandler.h>
#include <mc/server/ServerPlayer.h>

namespace translator {

LL_TYPE_INSTANCE_HOOK(
    PlayerConnectHook,
    HookPriority::Normal,
    ServerNetworkHandler,
    &ServerNetworkHandler::sendLoginMessageLocal,
    void,
    const NetworkIdentifier& networkIdentifier,
    const ConnectionRequest& connectionRequest,
    ServerPlayer&            player
) {
    MainManager::getAvailableCommandsPacket(player).sendToClient(networkIdentifier, player.getClientSubId());
    origin(networkIdentifier, connectionRequest, player);
}

void Hooks::setupHooks() { PlayerConnectHook::hook(); }

} // namespace translator