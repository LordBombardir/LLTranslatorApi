#include "Hooks.h"
#include "../manager/MainManager.h"
#include <ll/api/memory/Hook.h>
#include <mc/network/ServerNetworkHandler.h>
#include <mc/server/ServerPlayer.h>

namespace translator::hooks {

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
    origin(networkIdentifier, connectionRequest, player);
    manager::MainManager::getAvailableCommandsPacket(player).sendToClient(networkIdentifier, player.getClientSubId());
}

void Hooks::setupHooks() { PlayerConnectHook::hook(); }

} // namespace translator::hooks