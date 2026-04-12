#include "AddPlayerProcessor.h"
#include "../../core/MainManager.h"
#include "../../utils/Utils.h"
#include "../PlaceholdersManager.h"

#include <mc/entity/components/SynchedActorDataComponent.h>
#include <mc/network/packet/AddPlayerPacket.h>
#include <mc/world/actor/ActorLink.h>
#include <mc/world/actor/DataItem.h>
#include <mc/world/actor/SynchedActorDataEntityWrapper.h>
#include <mc/world/attribute/AttributeInstanceHandle.h>

namespace placeholder {

// Without cache
const Packet& AddPlayerProcessor::process(const NetworkIdentifier& id, const Packet& packet) const {
    const AddPlayerPacket& castedPacket = static_cast<const AddPlayerPacket&>(packet);

    AddPlayerPacket* newPacket    = new AddPlayerPacket();
    newPacket->mLinks             = castedPacket.mLinks;
    newPacket->mName              = castedPacket.mName;
    newPacket->mUuid              = castedPacket.mUuid;
    newPacket->mEntityId          = castedPacket.mEntityId;
    newPacket->mRuntimeId         = castedPacket.mRuntimeId;
    newPacket->mPlatformOnlineId  = castedPacket.mPlatformOnlineId;
    newPacket->mPos               = castedPacket.mPos;
    newPacket->mVelocity          = castedPacket.mVelocity;
    newPacket->mRot               = castedPacket.mRot;
    newPacket->mYHeadRot          = castedPacket.mYHeadRot;
    newPacket->mUnpack            = cloneDataItems(*castedPacket.mEntityData->mData->mData->mItemsArray);
    newPacket->mAbilities         = castedPacket.mAbilities;
    newPacket->mDeviceId          = castedPacket.mDeviceId;
    newPacket->mBuildPlatform     = castedPacket.mBuildPlatform;
    newPacket->mPlayerGameType    = castedPacket.mPlayerGameType;
    newPacket->mCarriedItem       = castedPacket.mCarriedItem;
    newPacket->mEntityData        = nullptr;
    newPacket->mSynchedProperties = castedPacket.mSynchedProperties;

    bool replacedSomething = false;
    for (auto& dataItem : *newPacket->mUnpack) {
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
        replaceDataItemStringValue(*newPacket->mUnpack, dataItem->getId(), data);

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