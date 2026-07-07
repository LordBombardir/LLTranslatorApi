#include "SetTitleProcessor.h"
#include "../../core/MainManager.h"

#include <mc/network/packet/SetTitlePacket.h>

SetTitlePacketPayload::SetTitlePacketPayload()                             = default;
SetTitlePacketPayload::SetTitlePacketPayload(const SetTitlePacketPayload&) = default;

namespace placeholder {

const Packet& SetTitleProcessor::process(const NetworkIdentifier& id, const Packet& packet) const {
    const SetTitlePacket& castedPacket = static_cast<const SetTitlePacket&>(packet);

    if (castedPacket.mTitleText->find(MainManager::getPrefixScope()) == std::string::npos) {
        return packet;
    }

    SetTitlePacket* newPacket = new SetTitlePacket(castedPacket);
    replaceAllPlaceholders(*newPacket->mTitleText, getAllPlaceholders(id));

    return *newPacket;
}

} // namespace placeholder