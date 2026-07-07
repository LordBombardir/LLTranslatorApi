#include "ShowModalFormRequestProcessor.h"
#include "../../core/MainManager.h"

#include <mc/network/packet/ModalFormRequestPacket.h>

namespace placeholder {

const Packet& ShowModalFormRequestProcessor::process(const NetworkIdentifier& id, const Packet& packet) const {
    const ModalFormRequestPacket& castedPacket = static_cast<const ModalFormRequestPacket&>(packet);

    if (castedPacket.mFormJSON.find(MainManager::getPrefixScope()) == std::string::npos) {
        return packet;
    }

    ModalFormRequestPacket* newPacket = new ModalFormRequestPacket(castedPacket);
    replaceAllPlaceholders(newPacket->mFormJSON, getAllPlaceholders(id));

    return *newPacket;
}

} // namespace placeholder