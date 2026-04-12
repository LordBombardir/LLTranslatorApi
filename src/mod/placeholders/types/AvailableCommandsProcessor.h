#pragma once

#include "../PlaceholderProcessor.h"

namespace placeholder {

class AvailableCommandsProcessor : public PlaceholderProcessor {
public:
    MinecraftPacketIds getPacketId() const override { return MinecraftPacketIds::AvailableCommands; }

    const Packet& process(const NetworkIdentifier& id, const Packet& packet) const override;
};

} // namespace placeholder