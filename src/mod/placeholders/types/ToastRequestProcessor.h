#pragma once

#include "../PlaceholderProcessor.h"

namespace placeholder {

class ToastRequestProcessor : public PlaceholderProcessor {
public:
    MinecraftPacketIds getPacketId() const override { return MinecraftPacketIds::ToastRequest; }

    const Packet& process(const NetworkIdentifier& id, const Packet& packet) const override;
};

} // namespace placeholder