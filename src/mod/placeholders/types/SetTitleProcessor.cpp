#include "SetTitleProcessor.h"
#include "../../core/MainManager.h"
#include "../../utils/Utils.h"
#include "../PlaceholdersManager.h"

#include <mc/network/packet/SetTitlePacket.h>

SetTitlePacketPayload::SetTitlePacketPayload()                             = default;
SetTitlePacketPayload::SetTitlePacketPayload(const SetTitlePacketPayload&) = default;

namespace placeholder {

const Packet& SetTitleProcessor::process(const NetworkIdentifier& id, const Packet& packet) const {
    const SetTitlePacket& castedPacket = static_cast<const SetTitlePacket&>(packet);

    const auto& allOccurrences = Utils::findAllOccurrences(*castedPacket.mTitleText, MainManager::getPrefixScope());
    if (allOccurrences.empty()) {
        return packet;
    }

    SetTitlePacket* newPacket = new SetTitlePacket(castedPacket);
    replaceAllPlaceholders(*newPacket->mTitleText, getAllPlaceholders(id), allOccurrences);

    PlaceholdersManager::addCachedPacket(&packet, newPacket, getPlayerLocaleCode(id));
    return *newPacket;
}

} // namespace placeholder