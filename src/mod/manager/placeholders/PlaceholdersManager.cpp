#include "PlaceholdersManager.h"
#include "../../utils/Utils.h"
#include "../MainManager.h"
#include <ll/api/service/Bedrock.h>
#include <mc/certificates/WebToken.h>
#include <mc/network/ConnectionRequest.h>
#include <mc/network/ServerNetworkHandler.h>
#include <mc/network/packet/AddActorPacket.h>
#include <mc/network/packet/AddPlayerPacket.h>
#include <mc/network/packet/AvailableCommandsPacket.h>
#include <mc/network/packet/ModalFormRequestPacket.h>
#include <mc/network/packet/SetActorDataPacket.h>
#include <mc/network/packet/SyncedAttribute.h>
#include <mc/network/packet/TextPacket.h>
#include <mc/world/actor/ActorLink.h>
#include <mc/world/actor/DataItem.h>
#include <mc/world/attribute/AttributeInstanceHandle.h>


AvailableCommandsPacket::EnumData::EnumData(const EnumData&)                                     = default;
AvailableCommandsPacket::SoftEnumData::SoftEnumData(const SoftEnumData&)                         = default;
AvailableCommandsPacket::ConstrainedValueData::ConstrainedValueData(const ConstrainedValueData&) = default;
AvailableCommandsPacket::ParamData::ParamData(const ParamData&)                                  = default;
AvailableCommandsPacket::OverloadData::OverloadData(const OverloadData&)                         = default;
AvailableCommandsPacket::CommandData::CommandData(const CommandData&)                            = default;

namespace translator::manager {

static constexpr short timeRemained = 60;

std::unordered_map<const Packet*, PlaceholdersManager::CachedPacket> PlaceholdersManager::cachedPackets = {};
std::mutex                                                           PlaceholdersManager::cachedPacketsMutex;

void PlaceholdersManager::cleanPacketsCache(bool forced) {
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

const Packet& PlaceholdersManager::processPacket(const NetworkIdentifier& id, const Packet& packet) {
    auto cachedPacket = getCachedPacket(&packet, getPlayerLocaleCode(id));
    if (cachedPacket != nullptr) {
        return *cachedPacket;
    }

    switch (packet.getId()) {
    case MinecraftPacketIds::AvailableCommands:
        return processAvailableCommandsPacket(id, packet);
    case MinecraftPacketIds::Text:
        return processTextPacket(id, packet);
    case MinecraftPacketIds::AddActor:
        return processAddActorPacket(id, packet);
    case MinecraftPacketIds::AddPlayer:
        return processAddPlayerPacket(id, packet);
    case MinecraftPacketIds::SetActorData:
        return processSetActorDataPacket(id, packet);
    case MinecraftPacketIds::ShowModalForm:
        return processShowModalFormRequestPacket(id, packet);
    default:
        return packet;
    }
}

void PlaceholdersManager::addCachedPacket(
    const Packet*      originalPacket,
    const Packet*      packet,
    const std::string& localeCode
) {
    std::lock_guard<std::mutex> lock(cachedPacketsMutex);

    PlaceholdersManager::CachedPacket cachedPacket;
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

// Without cache
const Packet& PlaceholdersManager::processAvailableCommandsPacket(const NetworkIdentifier& id, const Packet& packet) {
    AvailableCommandsPacket& castedPacket =
        const_cast<AvailableCommandsPacket&>(static_cast<const AvailableCommandsPacket&>(packet));

    for (AvailableCommandsPacket::CommandData& command : *castedPacket.mCommands) {
        const auto& placeholder = MainManager::getPlaceholder(command.name, getPlayerLocaleCode(id));
        if (placeholder.has_value()) {
            command.description = *placeholder;
        }
    }

    return castedPacket;
}

const Packet& PlaceholdersManager::processTextPacket(const NetworkIdentifier& id, const Packet& packet) {
    const TextPacket& castedPacket = static_cast<const TextPacket&>(packet);

    const auto& allOccurrences = Utils::findAllOccurrences(castedPacket.mMessage, MainManager::getPrefixScope());
    if (allOccurrences.empty()) {
        return packet;
    }

    TextPacket* newPacket = new TextPacket(castedPacket);
    replaceAllPlaceholders(newPacket->mMessage, getAllPlaceholders(id), allOccurrences);

    addCachedPacket(&packet, newPacket, getPlayerLocaleCode(id));
    return *newPacket;
}

// Without cache
const Packet& PlaceholdersManager::processAddActorPacket(const NetworkIdentifier& id, const Packet& packet) {
    AddActorPacket& castedPacket = const_cast<AddActorPacket&>(static_cast<const AddActorPacket&>(packet));

    for (auto& dataItem : *castedPacket.mData) {
        DataItemType type = dataItem->getType();
        if (type != DataItemType::String) {
            continue;
        }

        std::string data = dataItem->getData<std::string>();

        const auto& allOccurrences = Utils::findAllOccurrences(data, MainManager::getPrefixScope());
        if (allOccurrences.empty()) {
            return packet;
        }

        replaceAllPlaceholders(data, getAllPlaceholders(id), allOccurrences);
        replaceDataItemStringValue(*castedPacket.mData, dataItem->getId(), data);
    }

    return castedPacket;
}

// Without cache
const Packet& PlaceholdersManager::processAddPlayerPacket(const NetworkIdentifier& id, const Packet& packet) {
    AddPlayerPacket& castedPacket = const_cast<AddPlayerPacket&>(static_cast<const AddPlayerPacket&>(packet));

    for (auto& dataItem : *castedPacket.mUnpack) {
        DataItemType type = dataItem->getType();
        if (type != DataItemType::String) {
            continue;
        }

        std::string data = dataItem->getData<std::string>();

        const auto& allOccurrences = Utils::findAllOccurrences(data, MainManager::getPrefixScope());
        if (allOccurrences.empty()) {
            return packet;
        }

        replaceAllPlaceholders(data, getAllPlaceholders(id), allOccurrences);
        replaceDataItemStringValue(*castedPacket.mUnpack, dataItem->getId(), data);
    }

    return castedPacket;
}

// Without cache
const Packet& PlaceholdersManager::processSetActorDataPacket(const NetworkIdentifier& id, const Packet& packet) {
    SetActorDataPacket& castedPacket = const_cast<SetActorDataPacket&>(static_cast<const SetActorDataPacket&>(packet));

    for (auto& dataItem : castedPacket.mPackedItems) {
        DataItemType type = dataItem->getType();
        if (type != DataItemType::String) {
            continue;
        }

        std::string data = dataItem->getData<std::string>();

        const auto& allOccurrences = Utils::findAllOccurrences(data, MainManager::getPrefixScope());
        if (allOccurrences.empty()) {
            return packet;
        }

        replaceAllPlaceholders(data, getAllPlaceholders(id), allOccurrences);
        replaceDataItemStringValue(castedPacket.mPackedItems, dataItem->getId(), data);
    }

    return castedPacket;
}

// Without cache
const Packet&
PlaceholdersManager::processShowModalFormRequestPacket(const NetworkIdentifier& id, const Packet& packet) {
    ModalFormRequestPacket& castedPacket =
        const_cast<ModalFormRequestPacket&>(static_cast<const ModalFormRequestPacket&>(packet));

    const auto& allOccurrences = Utils::findAllOccurrences(*castedPacket.mFormJSON, MainManager::getPrefixScope());
    if (allOccurrences.empty()) {
        return packet;
    }

    replaceAllPlaceholders(*castedPacket.mFormJSON, getAllPlaceholders(id), allOccurrences);
    return castedPacket;
}

std::string PlaceholdersManager::getPlayerLocaleCode(const NetworkIdentifier& id) {
    auto serverNetworkHandler = ll::service::getServerNetworkHandler();
    if (!serverNetworkHandler) {
        return "";
    }

    auto& clients = *serverNetworkHandler->mClients;

    auto it = clients.find(id);
    if (it != clients.end()) {
        return it->second->mPrimaryRequest->mRawToken->mDataInfo["LanguageCode"].asString("");
    }

    return "";
}

std::unordered_map<std::string, std::string> PlaceholdersManager::getAllPlaceholders(const NetworkIdentifier& id) {
    const auto& localeCode = getPlayerLocaleCode(id);

    const auto& placeholders          = MainManager::getPlaceholders(localeCode);
    const auto& temporaryPlaceholders = MainManager::getTemporaryPlaceholders(localeCode);

    std::unordered_map<std::string, std::string> allPlaceholders = placeholders;
    allPlaceholders.insert(temporaryPlaceholders.begin(), temporaryPlaceholders.end());

    return allPlaceholders;
}

void PlaceholdersManager::replaceDataItemStringValue(
    std::vector<std::unique_ptr<DataItem>>& mData,
    ushort                                  id,
    const std::string&                      value
) {
    auto it = std::find_if(mData.begin(), mData.end(), [&id](const std::unique_ptr<DataItem>& item) -> bool {
        return item && item->getId() == id;
    });

    if (it == mData.end()) {
        return;
    }

    size_t index = std::distance(mData.begin(), it);
    mData[index] = DataItem::create(id, value);
}

void PlaceholdersManager::replaceAllPlaceholders(
    std::string&                                        value,
    const std::unordered_map<std::string, std::string>& placeholders,
    const std::vector<size_t>&                          allOccurrences
) {
    constexpr size_t prefixScopeLength = 16;
    constexpr size_t separatorLength   = 1;
    constexpr size_t keyLength         = 16;

    constexpr size_t totalKeyLength = prefixScopeLength + separatorLength + keyLength;

    bool replacedSomething = false;
    for (size_t pos : allOccurrences) {
        if (pos + totalKeyLength > value.size()) {
            continue;
        }

        std::string_view placeholder(value.data() + pos, totalKeyLength);

        auto it = placeholders.find(std::string(placeholder));
        if (it == placeholders.end()) {
            continue;
        }

        std::string newValue = Utils::strReplace(value, placeholder, it->second);
        if (newValue != value) {
            value             = std::move(newValue);
            replacedSomething = true;
        }
    }

    if (replacedSomething) {
        const auto& newOccurrences = Utils::findAllOccurrences(value, MainManager::getPrefixScope());
        if (!newOccurrences.empty()) {
            replaceAllPlaceholders(value, placeholders, newOccurrences);
        }
    }
}

} // namespace translator::manager