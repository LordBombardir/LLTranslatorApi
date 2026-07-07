#include "AddPlayerProcessor.h"
#include "../../core/MainManager.h"
#include "../PlaceholdersManager.h"

#include <cstring>
#include <mc/entity/components/SynchedActorDataComponent.h>
#include <mc/network/packet/AddPlayerPacket.h>
#include <mc/world/actor/ActorLink.h>
#include <mc/world/actor/DataItem.h>
#include <mc/world/actor/SynchedActorDataEntityWrapper.h>
#include <mc/world/attribute/AttributeInstanceHandle.h>

namespace placeholder {

Packet const& AddPlayerProcessor::process(NetworkIdentifier const& id, Packet const& packet) const {
    AddPlayerPacket const& castedPacket = static_cast<AddPlayerPacket const&>(packet);

    AddPlayerPacket* newPacket   = new AddPlayerPacket();
    newPacket->mLinks            = castedPacket.mLinks;
    newPacket->mName             = castedPacket.mName;
    newPacket->mUuid             = castedPacket.mUuid;
    newPacket->mEntityId         = castedPacket.mEntityId;
    newPacket->mRuntimeId        = castedPacket.mRuntimeId;
    newPacket->mPlatformOnlineId = castedPacket.mPlatformOnlineId;
    newPacket->mPos              = castedPacket.mPos;
    newPacket->mVelocity         = castedPacket.mVelocity;
    newPacket->mRot              = castedPacket.mRot;
    newPacket->mYHeadRot         = castedPacket.mYHeadRot;
    newPacket->mUnpack           = cloneDataItems(*castedPacket.mEntityData->mData->mData->mItemsArray);
    newPacket->mAbilities        = castedPacket.mAbilities;
    newPacket->mDeviceId         = castedPacket.mDeviceId;
    newPacket->mBuildPlatform    = castedPacket.mBuildPlatform;
    newPacket->mPlayerGameType   = castedPacket.mPlayerGameType;
    std::memcpy((void*)&newPacket->mCarriedItem, &castedPacket.mCarriedItem, sizeof(newPacket->mCarriedItem));
    newPacket->mEntityData        = nullptr;
    newPacket->mSynchedProperties = castedPacket.mSynchedProperties;

    std::string const prefixScope       = MainManager::getPrefixScope();
    bool              replacedSomething = false;
    for (auto& dataItem : *newPacket->mUnpack) {
        DataItemType type = dataItem->getType();
        if (type != DataItemType::String) {
            continue;
        }

        std::string data = dataItem->getData<std::string>();
        if (data.find(prefixScope) == std::string::npos) {
            continue;
        }

        replaceAllPlaceholders(data, getAllPlaceholders(id));
        replaceDataItemStringValue(*newPacket->mUnpack, dataItem->getId(), data);

        replacedSomething = true;
    }

    if (!replacedSomething) {
        delete newPacket;
        return packet;
    }

    return *newPacket;
}

} // namespace placeholder