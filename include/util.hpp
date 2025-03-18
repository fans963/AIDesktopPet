#pragma once

#include <cstdint>

namespace util
{
#define registerUniqueInterfaceId(variableName) static inline constexpr uint8_t variableName = __COUNTER__;
}
