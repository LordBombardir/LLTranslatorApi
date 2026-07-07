#include "PlaceholderProcessor.h"
#include "../config/ConfigManager.h"
#include "../config/types/Config.h"
#include "../core/MainManager.h"

#include <ll/api/service/Bedrock.h>
#include <mc/deps/certificates/WebToken.h>
#include <mc/network/ConnectionRequest.h>
#include <mc/network/ServerNetworkHandler.h>
#include <mc/world/actor/DataItem.h>

namespace placeholder {

std::string PlaceholderProcessor::getPlayerLocaleCode(const NetworkIdentifier& id) {
#ifdef LL_PLAT_C
    static bool isClientSide = true;
#else
    static bool isClientSide = false;
#endif

    auto serverNetworkHandler = ll::service::getServerNetworkHandler(isClientSide);
    if (!serverNetworkHandler) {
        return ConfigManager::getConfig().defaultLocaleCode;
    }

    auto& clients = *serverNetworkHandler->mClients;

    auto it = clients.find(id);
    if (it != clients.end()) {
        return it->second->mPrimaryRequest->mRawToken->mDataInfo["LanguageCode"].asString(
            ConfigManager::getConfig().defaultLocaleCode
        );
    }

    return ConfigManager::getConfig().defaultLocaleCode;
}

std::unordered_map<std::string, std::string>
PlaceholderProcessor::getAllPlaceholders(const NetworkIdentifier& id) const {
    const auto& localeCode = getPlayerLocaleCode(id);

    const auto& placeholders          = MainManager::getPlaceholders(localeCode);
    const auto& temporaryPlaceholders = MainManager::getTemporaryPlaceholders(localeCode);

    std::unordered_map<std::string, std::string> allPlaceholders = placeholders;
    allPlaceholders.insert(temporaryPlaceholders.begin(), temporaryPlaceholders.end());

    return allPlaceholders;
}

void PlaceholderProcessor::replaceAllPlaceholders(
    std::string&                                        value,
    const std::unordered_map<std::string, std::string>& placeholders
) const {
    const std::string prefixScope = MainManager::getPrefixScope();
    if (value.find(prefixScope) == std::string::npos) {
        return;
    }

    constexpr size_t prefixScopeLength = 16;
    constexpr size_t separatorLength   = 1;
    constexpr size_t keyLength         = 16;
    constexpr size_t totalKeyLength    = prefixScopeLength + separatorLength + keyLength;

    constexpr int maxDepth = 5;
    for (int depth = 0; depth < maxDepth; ++depth) {
        std::string result;
        result.reserve(value.size());

        bool   replacedSomething = false;
        size_t i                 = 0;
        while (i < value.size()) {
            if (i + totalKeyLength <= value.size() && value.compare(i, prefixScope.size(), prefixScope) == 0) {
                std::string placeholder = value.substr(i, totalKeyLength);
                auto        it          = placeholders.find(placeholder);
                if (it != placeholders.end()) {
                    result.append(it->second);
                    i += totalKeyLength;

                    replacedSomething = true;
                    continue;
                }
            }
            result.push_back(value[i]);
            i++;
        }

        if (!replacedSomething) {
            break;
        }

        value = std::move(result);
        if (value.find(prefixScope) == std::string::npos) {
            break;
        }
    }
}

std::vector<std::unique_ptr<DataItem>>
PlaceholderProcessor::cloneDataItems(const std::vector<std::unique_ptr<DataItem>>& source) const {
    std::vector<std::unique_ptr<DataItem>> result;
    result.reserve(source.size());

    for (const auto& item : source) {
        if (item) {
            result.push_back(item->clone());
        } else {
            result.push_back(DataItem::create(ActorDataIDs::Reserved0, 0));
        }
    }

    return result;
}

void PlaceholderProcessor::replaceDataItemStringValue(
    std::vector<std::unique_ptr<DataItem>>& mData,
    ushort                                  id,
    const std::string&                      value
) const {
    auto it = std::find_if(mData.begin(), mData.end(), [&id](const std::unique_ptr<DataItem>& item) -> bool {
        return item && item->getId() == id;
    });

    if (it == mData.end()) {
        return;
    }

    size_t index = std::distance(mData.begin(), it);

    static_cast<DataItem2<std::string>&>(*mData[index]).mValue = value;
}

} // namespace placeholder