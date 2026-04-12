#include "PlaceholdersManager.h"
#include "types/AddActorProcessor.h"
#include "types/AddPlayerProcessor.h"
#include "types/AvailableCommandsProcessor.h"
#include "types/SetActorDataProcessor.h"
#include "types/SetTitleProcessor.h"
#include "types/ShowModalFormRequestProcessor.h"
#include "types/TextProcessor.h"
#include "types/ToastRequestProcessor.h"

namespace placeholder {

static constexpr short timeRemained = 60;

std::unordered_map<const Packet*, PlaceholdersManager::CachedPacket> PlaceholdersManager::cachedPackets = {};
std::mutex                                                           PlaceholdersManager::cachedPacketsMutex;

std::vector<PlaceholdersManager::TemporaryPacket> PlaceholdersManager::temporaryPackets = {};
std::mutex                                        PlaceholdersManager::temporaryPacketsMutex;

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
    cleanCachedPackets(forced);
    cleanTemporaryPackets(forced);
}

void PlaceholdersManager::cleanCachedPackets(bool forced) {
    std::lock_guard<std::mutex> lock(cachedPacketsMutex);

    for (auto it = cachedPackets.begin(); it != cachedPackets.end();) {
        if (--it->second.secondsToCleanRemain <= 0 || forced) {
            for (const Packet* packet : it->second.packets | std::views::values) {
                delete packet;
            }

            it->second.packets.clear();
            it = cachedPackets.erase(it);
        }
    }
}

void PlaceholdersManager::cleanTemporaryPackets(bool forced) {
    std::lock_guard<std::mutex> lock(temporaryPacketsMutex);

    for (auto it = temporaryPackets.begin(); it != temporaryPackets.end();) {
        if (--it->secondsToCleanRemain <= 0 || forced) {
            delete it->packet;
            it = temporaryPackets.erase(it);
        }
    }
}

const Packet& PlaceholdersManager::processPacket(const NetworkIdentifier& id, const Packet& packet) {
    auto cachedPacket = getCachedPacket(&packet, PlaceholderProcessor::getPlayerLocaleCode(id));
    if (cachedPacket != nullptr) {
        return *cachedPacket;
    }

    auto processor = placeholderProcessors.find(packet.getId());
    if (processor == placeholderProcessors.end() || !processor->second) {
        return packet;
    }

    return processor->second->process(id, packet);
}

void PlaceholdersManager::addTemporaryPacket(const Packet* packet) {
    std::lock_guard<std::mutex> lock(temporaryPacketsMutex);
    temporaryPackets.emplace_back(timeRemained, packet);
}

void PlaceholdersManager::addCachedPacket(
    const Packet*      originalPacket,
    const Packet*      packet,
    const std::string& localeCode
) {
    std::lock_guard<std::mutex> lock(cachedPacketsMutex);

    PlaceholdersManager::CachedPacket cachedPacket;

    auto it = cachedPackets.find(originalPacket);
    if (it != cachedPackets.end()) {
        cachedPacket = std::move(it->second);
    }

    cachedPacket.secondsToCleanRemain = timeRemained;
    cachedPacket.packets[localeCode]  = packet;

    cachedPackets[originalPacket] = cachedPacket;
}

const Packet* PlaceholdersManager::getCachedPacket(const Packet* originalPacket, const std::string& localeCode) {
    std::lock_guard<std::mutex> lock(cachedPacketsMutex);

    auto firstIt = cachedPackets.find(originalPacket);
    if (firstIt == cachedPackets.end()) {
        return nullptr;
    }

    auto& cachedPacket                = firstIt->second;
    cachedPacket.secondsToCleanRemain = timeRemained;

    const auto& packets = cachedPacket.packets;

    auto secondIt = packets.find(localeCode);
    if (secondIt == packets.end()) {
        return nullptr;
    }

    return secondIt->second;
}

void PlaceholdersManager::registerProcessor(std::unique_ptr<PlaceholderProcessor> processor) {
    auto packetId = processor->getPacketId();
    placeholderProcessors.emplace(std::move(packetId), std::move(processor));
}

} // namespace placeholder
