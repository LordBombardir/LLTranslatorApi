#pragma once

#include <mc/network/Packet.h>

class DataItem;

namespace placeholder {

class PlaceholderProcessor {
public:
    virtual ~PlaceholderProcessor() = default;

    virtual MinecraftPacketIds getPacketId() const = 0;

    virtual const Packet& process(const NetworkIdentifier& id, const Packet& packet) const = 0;
    
    static std::string getPlayerLocaleCode(const NetworkIdentifier& id);

protected:
    std::unordered_map<std::string, std::string> getAllPlaceholders(const NetworkIdentifier& id) const;

    void replaceAllPlaceholders(
        std::string&                                        value,
        const std::unordered_map<std::string, std::string>& placeholders,
        const std::vector<size_t>&                          allOccurrences
    ) const;

    std::vector<std::unique_ptr<DataItem>> cloneDataItems(const std::vector<std::unique_ptr<DataItem>>& source) const;
    void replaceDataItemStringValue(std::vector<std::unique_ptr<DataItem>>& mData, ushort id, const std::string& value) const;
};

} // namespace placeholder