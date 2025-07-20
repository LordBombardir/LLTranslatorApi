#pragma once

#include <functional>

namespace translator::tasks {

class CacheCleanerTask final {
public:
    static bool init();
    static bool isAlreadyStarted();

private:
    static bool isStarted;

    static std::function<void()> function;
};

} // namespace regions::tasks