#pragma once

#include <cstdint>
#include <functional>
#include <chrono>
#include <future>
#include "log.hpp"

#define registerUniqueInterfaceId(variableName)                 \
    static inline constexpr uint8_t variableName = __COUNTER__; \
    static_assert(__COUNTER__ < 255, "Interface id must be less than 255");

namespace util
{
    template <typename Func>
    static inline bool createTimeOutTask(Func &&func, const uint16_t&ms)
    {
        static_assert(std::is_same<decltype(func()),bool>::value,"Task func must return bool");
        std::packaged_task<bool()> task(std::bind(std::forward<Func>(func)));
        std::future<bool> future = task.get_future();
       
        std::thread(std::move(task)).detach();

        if (future.wait_for(std::chrono::milliseconds{ms}) == std::future_status::ready)
            return future.get();
        else
        {
            logger::warn("Timeout");
            return false;
        }
    }
}
