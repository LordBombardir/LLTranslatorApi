#pragma once

#include "../PlaceholderProcessor.h"

namespace placeholder {

class AddActorProcessor : public PlaceholderProcessor {
public:
    MinecraftPacketIds getPacketId() const override { return MinecraftPacketIds::AddActor; }

    const Packet& process(const NetworkIdentifier& id, const Packet& packet) const override;
};

} // namespace placeholder