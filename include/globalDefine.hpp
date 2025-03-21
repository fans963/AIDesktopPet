#pragma once
#include <string>
#include "util.hpp"

namespace global
{
    static inline constexpr char *wifiSsid{"test"};
    static inline constexpr char *wifiPassword{"test"};
    static inline constexpr char *deepseekApiKey{"test"};
    static inline constexpr char *deepseekApiUrl{"test"};

    static inline constexpr auto  wifiStatusId= util::next();
}