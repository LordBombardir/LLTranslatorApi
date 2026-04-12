#include "../../core/MainManager.h"
#include "../../utils/Utils.h"
#include "../PlaceholdersManager.h"
#include "ShowModalFormRequestProcessor.h"

#include <mc/network/packet/ModalFormRequestPacket.h>

namespace placeholder {

// Without cache (const_cast)
const Packet& ShowModalFormRequestProcessor::process(const NetworkIdentifier& id, const Packet& packet) const {
    ModalFormRequestPacket& castedPacket =
        const_cast<ModalFormRequestPacket&>(static_cast<const ModalFormRequestPacket&>(packet));

    const auto& allOccurrences = Utils::findAllOccurrences(castedPacket.mFormJSON, MainManager::getPrefixScope());
    if (allOccurrences.empty()) {
        return packet;
    }

    replaceAllPlaceholders(castedPacket.mFormJSON, getAllPlaceholders(id), allOccurrences);
    return castedPacket;
}

} // namespace placeholder