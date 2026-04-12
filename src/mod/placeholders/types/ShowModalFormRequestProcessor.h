#pragma once

#include "../PlaceholderProcessor.h"

namespace placeholder {

class ShowModalFormRequestProcessor : public PlaceholderProcessor {
public:
    MinecraftPacketIds getPacketId() const override { return MinecraftPacketIds::ShowModalForm; }

    const Packet& process(const NetworkIdentifier& id, const Packet& packet) const override;
};

} // namespace placeholder