#pragma once

#include <string>

namespace placeholder::config {

struct Config {
    int         version           = 2;
    std::string defaultLocaleCode = "en_US";
};

} // namespace placeholder::config