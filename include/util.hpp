#pragma once

#include <cstdint>
#include <functional>
#include <chrono>
#include <future>
#include "log.hpp"

namespace util
{
    template <uint8_t N>
    struct flag
    {
        friend constexpr uint8_t adlFlag(flag<N>);
    };

    template <uint8_t N>
    struct writer
    {
        friend constexpr uint8_t adlFlag(flag<N>)
        {
            return N;
        }
        static constexpr uint8_t value = N;
    };

    template <uint8_t N, uint8_t = adlFlag(flag<N>{})>
    uint8_t constexpr reader(uint8_t, flag<N>)
    {
        return N;
    }

    template <uint8_t N>
    uint8_t constexpr reader(float, flag<N>, uint8_t R = reader(0, flag<N - 1>{}))
    {
        return R;
    }

    uint8_t constexpr reader(float, flag<0>)
    {
        return 0;
    }

    template <uint8_t N = 1>
    uint8_t constexpr next(uint8_t R = writer<reader(0, flag<255>{}) + N>::value)
    {
        return R;
    }

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
