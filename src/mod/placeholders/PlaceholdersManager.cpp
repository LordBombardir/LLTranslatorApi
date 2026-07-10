#include "PlaceholdersManager.h"
#include "types/AddActorProcessor.h"
#include "types/AddPlayerProcessor.h"
#include "types/AvailableCommandsProcessor.h"
#include "types/SetActorDataProcessor.h"
#include "types/SetTitleProcessor.h"
#include "types/ShowModalFormRequestProcessor.h"
#include "types/TextProcessor.h"
#include "types/ToastRequestProcessor.h"

#include <ranges>

namespace placeholder {

thread_local int sendNestingDepth = 0;

std::unordered_map<const Packet*, PlaceholdersManager::CachedPacket> PlaceholdersManager::cachedPackets = {};
std::mutex                                                           PlaceholdersManager::cachedPacketsMutex;

std::unordered_map<MinecraftPacketIds, std::unique_ptr<PlaceholderProcessor>>
    PlaceholdersManager::placeholderProcessors = {};

void PlaceholdersManager::init() {
    registerProcessor(std::make_unique<AvailableCommandsProcessor>());
    registerProcessor(std::make_unique<TextProcessor>());
    registerProcessor(std::make_unique<SetTitleProcessor>());
    registerProcessor(std::make_unique<ToastRequestProcessor>());
    registerProcessor(std::make_unique<AddActorProcessor>());
    registerProcessor(std::make_unique<AddPlayerProcessor>());
    registerProcessor(std::make_unique<SetActorDataProcessor>());
    registerProcessor(std::make_unique<ShowModalFormRequestProcessor>());
}

void PlaceholdersManager::cleanPackets(bool forced) {
    if (forced) {
        std::lock_guard<std::mutex> lock(cachedPacketsMutex);
        for (const auto& cached : cachedPackets | std::views::values) {
            for (const Packet* packet : cached.packets | std::views::values) {
                delete packet;
            }
        }

        cachedPackets.clear();
    }
}

const Packet& PlaceholdersManager::processPacket(const NetworkIdentifier& id, const Packet& packet) {
    const std::string localeCode = PlaceholderProcessor::getPlayerLocaleCode(id);

    if (isInsideSendToMultiple()) {
        auto cachedPacket = getCachedPacket(&packet, localeCode);
        if (cachedPacket != nullptr) {
            return *cachedPacket;
        }
    }

    auto processor = placeholderProcessors.find(packet.getId());
    if (processor == placeholderProcessors.end() || !processor->second) {
        return packet;
    }

    const Packet& processed = processor->second->process(id, packet);
    if (&processed != &packet) {
        addCachedPacket(&packet, &processed, localeCode);
    }

    return processed;
}

void PlaceholdersManager::addCachedPacket(
    const Packet*      originalPacket,
    const Packet*      packet,
    const std::string& localeCode
) {
    std::lock_guard<std::mutex> lock(cachedPacketsMutex);
    cachedPackets[originalPacket].packets[localeCode] = packet;
}

const Packet* PlaceholdersManager::getCachedPacket(const Packet* originalPacket, const std::string& localeCode) {
    std::lock_guard<std::mutex> lock(cachedPacketsMutex);

    auto firstIt = cachedPackets.find(originalPacket);
    if (firstIt == cachedPackets.end()) {
        return nullptr;
    }

    const auto& packets  = firstIt->second.packets;
    auto        secondIt = packets.find(localeCode);
    if (secondIt == packets.end()) {
        return nullptr;
    }

    return secondIt->second;
}

void PlaceholdersManager::startSendScope() { sendNestingDepth++; }

void PlaceholdersManager::endSendScope() {
    if (--sendNestingDepth == 0) {
        cleanPackets(true);
    }
}

bool PlaceholdersManager::isInsideSendToMultiple() { return sendNestingDepth > 1; }

void PlaceholdersManager::registerProcessor(std::unique_ptr<PlaceholderProcessor> processor) {
    auto packetId = processor->getPacketId();
    placeholderProcessors.emplace(std::move(packetId), std::move(processor));
}

} // namespace placeholder
