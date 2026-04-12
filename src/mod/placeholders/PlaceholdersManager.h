#pragma once

#include "PlaceholderProcessor.h"

#include <mc/network/Packet.h>
#include <unordered_map>

class DataItem;

namespace placeholder {

class PlaceholdersManager final {
public:
    static void init();
    static void cleanPackets(bool forced = false);

    static const Packet& processPacket(const NetworkIdentifier& id, const Packet& packet);

    static void addTemporaryPacket(const Packet* packet);
    static void addCachedPacket(const Packet* originalPacket, const Packet* packet, const std::string& localeCode);

private:
    struct CachedPacket {
        short                                          secondsToCleanRemain;
        std::unordered_map<std::string, const Packet*> packets;
    };

    static std::unordered_map<const Packet*, CachedPacket> cachedPackets;
    static std::mutex                                      cachedPacketsMutex;

    struct TemporaryPacket {
        short         secondsToCleanRemain;
        const Packet* packet;
    };

    static std::vector<TemporaryPacket> temporaryPackets;
    static std::mutex                   temporaryPacketsMutex;

    static std::unordered_map<MinecraftPacketIds, std::unique_ptr<PlaceholderProcessor>> placeholderProcessors;

    static void cleanCachedPackets(bool forced);
    static void cleanTemporaryPackets(bool forced);

    static const Packet* getCachedPacket(const Packet* originalPacket, const std::string& localeCode);

    static void registerProcessor(std::unique_ptr<PlaceholderProcessor> processor);
};

} // namespace placeholder