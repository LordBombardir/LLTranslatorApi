#pragma once

#include "../PlaceholderProcessor.h"

namespace placeholder {

class AddPlayerProcessor : public PlaceholderProcessor {
public:
    MinecraftPacketIds getPacketId() const override { return MinecraftPacketIds::AddPlayer; }

    const Packet& process(const NetworkIdentifier& id, const Packet& packet) const override;
};

} // namespace placeholder