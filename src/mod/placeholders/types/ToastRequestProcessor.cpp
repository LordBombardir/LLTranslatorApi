#include "../../core/MainManager.h"
#include "../../utils/Utils.h"
#include "../PlaceholdersManager.h"
#include "ToastRequestProcessor.h"

#include <mc/network/packet/ToastRequestPacket.h>

ToastRequestPacketPayload::ToastRequestPacketPayload()                                 = default;
ToastRequestPacketPayload::ToastRequestPacketPayload(const ToastRequestPacketPayload&) = default;

namespace placeholder {

const Packet& ToastRequestProcessor::process(const NetworkIdentifier& id, const Packet& packet) const {
    const ToastRequestPacket& castedPacket = static_cast<const ToastRequestPacket&>(packet);

    const auto& firstAllOccurrences  = Utils::findAllOccurrences(*castedPacket.mTitle, MainManager::getPrefixScope());
    const auto& secondAllOccurrences = Utils::findAllOccurrences(*castedPacket.mContent, MainManager::getPrefixScope());

    if (firstAllOccurrences.empty() && secondAllOccurrences.empty()) {
        return packet;
    }

    ToastRequestPacket* newPacket = new ToastRequestPacket(castedPacket);

    replaceAllPlaceholders(*newPacket->mTitle, getAllPlaceholders(id), firstAllOccurrences);
    replaceAllPlaceholders(*newPacket->mContent, getAllPlaceholders(id), secondAllOccurrences);

    PlaceholdersManager::addCachedPacket(&packet, newPacket, getPlayerLocaleCode(id));
    return *newPacket;
}

} // namespace placeholder