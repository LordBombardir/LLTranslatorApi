#include "AddActorProcessor.h"
#include "../../core/MainManager.h"
#include "../PlaceholdersManager.h"

#include <mc/network/packet/AddActorPacket.h>
#include <mc/world/actor/ActorLink.h>
#include <mc/world/actor/DataItem.h>
#include <mc/world/actor/SynchedActorDataEntityWrapper.h>
#include <mc/world/attribute/AttributeInstanceHandle.h>
#include <mc/network/packet/SyncedAttribute.h>

PropertySyncData& PropertySyncData::operator=(const PropertySyncData&) = default;
PropertySyncData::PropertySyncData()                                   = default;

namespace placeholder {

const Packet& AddActorProcessor::process(const NetworkIdentifier& id, const Packet& packet) const {
    const AddActorPacket& castedPacket = static_cast<const AddActorPacket&>(packet);

    AddActorPacket* newPacket     = new AddActorPacket();
    newPacket->mLinks             = castedPacket.mLinks;
    newPacket->mPos               = castedPacket.mPos;
    newPacket->mVelocity          = castedPacket.mVelocity;
    newPacket->mRot               = castedPacket.mRot;
    newPacket->mYHeadRotation     = castedPacket.mYHeadRotation;
    newPacket->mYBodyRotation     = castedPacket.mYBodyRotation;
    newPacket->mEntityId          = castedPacket.mEntityId;
    newPacket->mRuntimeId         = castedPacket.mRuntimeId;
    newPacket->mData              = cloneDataItems(castedPacket.mData);
    newPacket->mType              = castedPacket.mType;
    newPacket->mAttributes        = castedPacket.mAttributes;
    newPacket->mSynchedProperties = castedPacket.mSynchedProperties;
    newPacket->mAttributeHandles  = castedPacket.mAttributeHandles;
    newPacket->mMap               = castedPacket.mMap;
    newPacket->mEntityData        = castedPacket.mEntityData;

    const std::string prefixScope = MainManager::getPrefixScope();
    bool replacedSomething = false;
    for (auto& dataItem : *newPacket->mData) {
        DataItemType type = dataItem->getType();
        if (type != DataItemType::String) {
            continue;
        }

        std::string data = dataItem->getData<std::string>();
        if (data.find(prefixScope) == std::string::npos) {
            continue;
        }

        replaceAllPlaceholders(data, getAllPlaceholders(id));
        replaceDataItemStringValue(*newPacket->mData, dataItem->getId(), data);

        replacedSomething = true;
    }

    if (!replacedSomething) {
        delete newPacket;
        return packet;
    }

    return *newPacket;
}

} // namespace placeholder