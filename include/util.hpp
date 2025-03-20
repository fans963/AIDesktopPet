#pragma once

#include <cstdint>
#include <functional>
#include <chrono>

namespace util
{
#define registerUniqueInterfaceId(variableName) static inline constexpr uint8_t variableName = __COUNTER__;

//可以用但尽量不要用，因为会阻塞。采用硬件定时器有更好的实现，但是要用多线程，感觉用硬件定时器不太安全
    template <typename returnType, typename... Args, typename Func>
    static inline returnType createTimeOutTask(Func&& task, std::chrono::milliseconds timeout, const returnType &defaultValue, Args &&...args)
    {
        const auto startTime = std::chrono::steady_clock::now();
        while (std::chrono::steady_clock::now() - startTime < timeout)
            return task(std::forward<Args>(args)...);
        return defaultValue;
    }
}
