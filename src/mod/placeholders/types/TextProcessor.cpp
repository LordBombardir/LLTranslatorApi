#include "TextProcessor.h"
#include "../../core/MainManager.h"
#include "../../utils/Utils.h"
#include "../PlaceholdersManager.h"

#include <mc/network/packet/TextPacket.h>

namespace placeholder {

// Without cache
const Packet& TextProcessor::process(const NetworkIdentifier& id, const Packet& packet) const {
    const TextPacket& castedPacket = static_cast<const TextPacket&>(packet);

    const auto& allOccurrences = Utils::findAllOccurrences(castedPacket.getMessage(), MainManager::getPrefixScope());
    if (allOccurrences.empty()) {
        return packet;
    }

    TextPacket* newPacket = new TextPacket(castedPacket);
    std::visit(
        [&](auto& payload) -> void {
            replaceAllPlaceholders(payload.mMessage, getAllPlaceholders(id), allOccurrences);
        },
        *newPacket->mBody
    );

    PlaceholdersManager::addTemporaryPacket(newPacket);
    return *newPacket;
}

} // namespace placeholder