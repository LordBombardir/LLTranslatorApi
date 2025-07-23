#pragma once

#include <mc/client/renderer/rendergraph/Packet.h>
#include <unordered_map>

class DataItem;

namespace translator::manager {

class PlaceholdersManager final {
public:
    static void cleanPacketsCache(bool forced = false);

    static const Packet& processPacket(const NetworkIdentifier& id, const Packet& packet);

private:
    struct CachedPacket {
        short                                          secondsToCleanRemain;
        std::unordered_map<std::string, const Packet*> packets;
    };

    static std::unordered_map<const Packet*, CachedPacket> cachedPackets;
    static std::mutex                                      cachedPacketsMutex;

    static void addCachedPacket(const Packet* originalPacket, const Packet* packet, const std::string& localeCode);
    static const Packet* getCachedPacket(const Packet* originalPacket, const std::string& localeCode);

    static const Packet& processAvailableCommandsPacket(const NetworkIdentifier& id, const Packet& packet);
    static const Packet& processTextPacket(const NetworkIdentifier& id, const Packet& packet);
    static const Packet& processAddActorPacket(const NetworkIdentifier& id, const Packet& packet);
    static const Packet& processAddPlayerPacket(const NetworkIdentifier& id, const Packet& packet);
    static const Packet& processSetActorDataPacket(const NetworkIdentifier& id, const Packet& packet);
    static const Packet& processShowModalFormRequestPacket(const NetworkIdentifier& id, const Packet& packet);

    static std::string getPlayerLocaleCode(const NetworkIdentifier& id);

    static std::unordered_map<std::string, std::string> getAllPlaceholders(const NetworkIdentifier& id);

    static void
    replaceDataItemStringValue(std::vector<std::unique_ptr<DataItem>>& mData, ushort id, const std::string& value);
    static void replaceAllPlaceholders(
        std::string&                                         value,
        const std::unordered_map<std::string, std::string>&  placeholders,
        const std::vector<std::string_view::const_iterator>& allOccurrences
    );
};

} // namespace translator::manager