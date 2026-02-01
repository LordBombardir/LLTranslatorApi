#include "CacheCleanerTask.h"
#include "../manager/MainManager.h"
#include "../manager/placeholders/PlaceholdersManager.h"
#include <ll/api/coro/CoroTask.h>
#include <ll/api/thread/ServerThreadExecutor.h>

namespace placeholder::tasks {

bool CacheCleanerTask::isStarted = false;

std::function<void()> CacheCleanerTask::function = {};

bool CacheCleanerTask::init() {
    if (isStarted) {
        return false;
    }

    function = []() -> void {
        manager::PlaceholdersManager::cleanPackets();
        manager::MainManager::cleanTemporaryPlaceholders();
    };

    using namespace std::chrono_literals;
    ll::coro::keepThis([function = &function]() -> ll::coro::CoroTask<> {
        while (true) {
            co_await 1s;
            (*function)();
        }
        co_return;
    }).launch(ll::thread::ServerThreadExecutor::getDefault());

    isStarted = true;
    return true;
}

bool CacheCleanerTask::isAlreadyStarted() { return isStarted; }

} // namespace placeholder::tasks