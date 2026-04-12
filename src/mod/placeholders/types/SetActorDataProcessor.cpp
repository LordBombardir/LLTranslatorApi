#include "SetActorDataProcessor.h"
#include "../../core/MainManager.h"
#include "../../utils/Utils.h"
#include "../PlaceholdersManager.h"

#include <mc/entity/components/SynchedActorDataComponent.h>
#include <mc/network/packet/SetActorDataPacket.h>
#include <mc/world/actor/DataItem.h>

SetActorDataPacket::SetActorDataPacket() = default;

namespace placeholder {

// Without cache
const Packet& SetActorDataProcessor::process(const NetworkIdentifier& id, const Packet& packet) const {
    const SetActorDataPacket& castedPacket = static_cast<const SetActorDataPacket&>(packet);

    SetActorDataPacket* newPacket = new SetActorDataPacket();
    newPacket->mId                = castedPacket.mId;
    newPacket->mPackedItems       = cloneDataItems(castedPacket.mPackedItems);
    newPacket->mSynchedProperties = castedPacket.mSynchedProperties;
    newPacket->mTick              = castedPacket.mTick;

    bool replacedSomething = false;
    for (auto& dataItem : newPacket->mPackedItems) {
        DataItemType type = dataItem->getType();
        if (type != DataItemType::String) {
            continue;
        }

        std::string data = dataItem->getData<std::string>();

        const auto& allOccurrences = Utils::findAllOccurrences(data, MainManager::getPrefixScope());
        if (allOccurrences.empty()) {
            continue;
        }

        replaceAllPlaceholders(data, getAllPlaceholders(id), allOccurrences);
        replaceDataItemStringValue(newPacket->mPackedItems, dataItem->getId(), data);

        replacedSomething = true;
    }

    if (!replacedSomething) {
        delete newPacket;
        return packet;
    }

    PlaceholdersManager::addTemporaryPacket(newPacket);
    return *newPacket;
}

} // namespace placeholder