#pragma once

#include <atomic>
#include <thread>
#include <WiFi.h>

#include "core/component.hpp"
#include "globalDefine.hpp"
#include "util.hpp"
#include "log.hpp"

namespace network
{
    using namespace core;
    using namespace global;
    using namespace logger;
    class WIFI : public Component
    {
    public:
        void beforeUpdate() override
        {
            registerOutput(wifiStatusId,wifiStatus_);
            connectThread_.detach();
        }

        //warning:其他地方莫要用WiFi.Status()，否则可能会出现严重的问题，registerInput那个wifiStatus_来判断WIFI连接状况
        void update() override {
            if (connected_.load(std::memory_order::memory_order_relaxed) && WiFi.status() != WL_CONNECTED)
            {
                connected_.store(false, std::memory_order::memory_order_relaxed);
                *wifiStatus_ = false;
                connectThread_.detach();
            }
        }

    private:
        void tryConnect()
        {
            while (true)
            {
                if (WiFi.status() != WL_CONNECTED)
                {
                    WiFi.begin(wifiSsid, wifiPassword);
                    info("Connecting to WiFi:%s with password:%s ...", wifiSsid, wifiPassword);
                    delay(500);
                }
                else
                {
                    connected_.store(true, std::memory_order::memory_order_relaxed);
                    *wifiStatus_ = true;
                    info("Connected to WiFi:%s", wifiSsid);
                    break;
                }
            }
        }

        std::thread connectThread_{[this]()
                                   {
                                       tryConnect();
                                   }};
        std::atomic<bool> connected_{false};
        OutputInterface<bool> wifiStatus_;
    };
}