#pragma once

#include "PlaceholderProcessor.h"

#include <mc/network/Packet.h>
#include <memory>
#include <mutex>
#include <unordered_map>

class DataItem;

namespace placeholder {

class PlaceholdersManager final {
public:
    class SendScopeGuard {
    public:
        SendScopeGuard() { startSendScope(); }
        ~SendScopeGuard() { endSendScope(); }
    };

    static void init();
    static void cleanPackets(bool forced = false);

    static const Packet& processPacket(const NetworkIdentifier& id, const Packet& packet);

    static void addCachedPacket(const Packet* originalPacket, const Packet* packet, const std::string& localeCode);

    static void startSendScope();
    static void endSendScope();
    static bool isInsideSendToMultiple();

private:
    struct CachedPacket {
        std::unordered_map<std::string, const Packet*> packets;
    };

    static std::unordered_map<const Packet*, CachedPacket> cachedPackets;
    static std::mutex                                      cachedPacketsMutex;

    static std::unordered_map<MinecraftPacketIds, std::unique_ptr<PlaceholderProcessor>> placeholderProcessors;

    static const Packet* getCachedPacket(const Packet* originalPacket, const std::string& localeCode);

    static void registerProcessor(std::unique_ptr<PlaceholderProcessor> processor);
};

} // namespace placeholder