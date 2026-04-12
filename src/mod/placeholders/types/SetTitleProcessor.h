#pragma once

#include "../PlaceholderProcessor.h"

namespace placeholder {

class SetTitleProcessor : public PlaceholderProcessor {
public:
    MinecraftPacketIds getPacketId() const override { return MinecraftPacketIds::SetTitle; }

    const Packet& process(const NetworkIdentifier& id, const Packet& packet) const override;
};

} // namespace placeholder