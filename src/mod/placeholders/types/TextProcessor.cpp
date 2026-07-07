#include "TextProcessor.h"
#include "../../core/MainManager.h"

#include <mc/network/packet/TextPacket.h>

namespace placeholder {

const Packet& TextProcessor::process(const NetworkIdentifier& id, const Packet& packet) const {
    const TextPacket& castedPacket = static_cast<const TextPacket&>(packet);

    if (castedPacket.getMessage().find(MainManager::getPrefixScope()) == std::string::npos) {
        return packet;
    }

    TextPacket* newPacket = new TextPacket(castedPacket);
    std::visit(
        [&](auto& payload) -> void { replaceAllPlaceholders(payload.mMessage, getAllPlaceholders(id)); },
        *newPacket->mBody
    );

    return *newPacket;
}

} // namespace placeholder