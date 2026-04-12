#include "AvailableCommandsProcessor.h"
#include "../../core/MainManager.h"

#include <mc/network/packet/AvailableCommandsPacket.h>

AvailableCommandsPacket::ParamData::ParamData(const AvailableCommandsPacket::ParamData&) = default;

AvailableCommandsPacket::CommandData::CommandData(const CommandData&) = default;

namespace placeholder {

// Without cache (const_cast)
const Packet& AvailableCommandsProcessor::process(const NetworkIdentifier& id, const Packet& packet) const {
    AvailableCommandsPacket& castedPacket =
        const_cast<AvailableCommandsPacket&>(static_cast<const AvailableCommandsPacket&>(packet));

    for (AvailableCommandsPacket::CommandData& command : *castedPacket.mCommands) {
        const auto& placeholder = MainManager::getPlaceholder(command.name, getPlayerLocaleCode(id));
        if (placeholder.has_value()) {
            command.description = *placeholder;
        }
    }

    return castedPacket;
}

} // namespace placeholder