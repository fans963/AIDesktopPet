#pragma once

#include <thread>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "core/component.hpp"

namespace network
{
    using namespace core;
    class VoiceToString:public Component{
    public:
        void beforeUpdate()override{}

        void update()override{}
    private:
        std::thread requestThread_;
    };
}