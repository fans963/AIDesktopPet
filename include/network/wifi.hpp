#pragma once

#include <atomic>
#include <thread>
#include <WiFi.h>
#include <core/component.hpp>
#include "globalDefine.hpp"
#include "util.hpp"
#include "log.hpp"

namespace network{
    using namespace core;
    using namespace global;
    using namespace logger;
    class WIFI : public Component
    {
        public:
            void beforeUpdate()override{
                connectThread_.detach();
            }
            void update()override{}
        private:
        void tryConnect(){
            while (true)
            {
                if (WiFi.status() != WL_CONNECTED)
                {
                    WiFi.begin(wifiSsid, wifiPassword);
                    info("Connecting to WiFi:%s with password:%s ...", wifiSsid,wifiPassword);
                    delay(500);
                }
                else
                {
                    connected_.store(true, std::memory_order::memory_order_relaxed);
                    break;
                }
            }
            
        }

        std::thread connectThread_{[this]()
                                   {
                                       tryConnect();
                                   }};
        std::atomic<bool> connected_{false};
    };
}