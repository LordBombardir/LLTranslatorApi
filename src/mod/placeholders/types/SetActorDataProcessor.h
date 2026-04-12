#pragma once

#include "../PlaceholderProcessor.h"

namespace placeholder {

class SetActorDataProcessor : public PlaceholderProcessor {
public:
    MinecraftPacketIds getPacketId() const override { return MinecraftPacketIds::SetActorData; }

    const Packet& process(const NetworkIdentifier& id, const Packet& packet) const override;
};

} // namespace placeholder