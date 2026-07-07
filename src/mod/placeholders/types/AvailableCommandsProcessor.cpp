#include "AvailableCommandsProcessor.h"
#include "../../core/MainManager.h"

#include <mc/network/packet/AvailableCommandsPacket.h>

namespace placeholder {

const Packet& AvailableCommandsProcessor::process(const NetworkIdentifier& id, const Packet& packet) const {
    const AvailableCommandsPacket& castedPacket = static_cast<const AvailableCommandsPacket&>(packet);

    AvailableCommandsPacket* newPacket = new AvailableCommandsPacket(castedPacket);

    bool replacedSomething = false;
    for (AvailableCommandsPacketPayload::CommandData& command : *newPacket->mCommands) {
        const auto& placeholder = MainManager::getPlaceholder(command.name, getPlayerLocaleCode(id));
        if (placeholder.has_value()) {
            command.description = *placeholder;
            replacedSomething = true;
        }
    }

    if (!replacedSomething) {
        delete newPacket;
        return packet;
    }

    return *newPacket;
}

} // namespace placeholder