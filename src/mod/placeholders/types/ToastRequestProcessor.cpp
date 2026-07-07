#include "ToastRequestProcessor.h"
#include "../../core/MainManager.h"

#include <mc/network/packet/ToastRequestPacket.h>

ToastRequestPacketPayload::ToastRequestPacketPayload()                                 = default;
ToastRequestPacketPayload::ToastRequestPacketPayload(const ToastRequestPacketPayload&) = default;

namespace placeholder {

const Packet& ToastRequestProcessor::process(const NetworkIdentifier& id, const Packet& packet) const {
    const ToastRequestPacket& castedPacket = static_cast<const ToastRequestPacket&>(packet);

    const std::string prefixScope = MainManager::getPrefixScope();
    if (castedPacket.mTitle->find(prefixScope) == std::string::npos
        && castedPacket.mContent->find(prefixScope) == std::string::npos) {
        return packet;
    }

    ToastRequestPacket* newPacket = new ToastRequestPacket(castedPacket);

    replaceAllPlaceholders(*newPacket->mTitle, getAllPlaceholders(id));
    replaceAllPlaceholders(*newPacket->mContent, getAllPlaceholders(id));

    return *newPacket;
}

} // namespace placeholder