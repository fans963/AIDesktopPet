#pragma once

#include <thread>
#include <HTTPClient.h>

#include "core/component.hpp"
#include "globalDefine.hpp"

namespace network{
    using namespace core;
    using namespace global;
    class AI : public Component
    {
        public:
        void beforeUpdate()override{
            registerInput(wifiStatusId, wifiStatus_);
        }
        void update() override{
            auto aiThread = std::thread{[this](){}};
        }

        private:
        static inline void voiceToString(){}
        static inline const char *jsonBody(const std::string &descriptor, const std::string &content)
        {
            const std::string jsonBody = "{\"model\":\"deepseek-reasoner\",\"messages\":[{\"role\":\"system\",\"content\":\"" + descriptor + "\"},{\"role\":\"user\",\"content\":\"" + content + "\"}],\"stream\":false}";
            return jsonBody.c_str();
        }
        static inline void ai(){
            http_.begin(deepseekApiUrl);
            http_.addHeader("Content-Type", "application/json");
            http_.addHeader("Authorization",deepseekApiKey);
            http_.POST(jsonBody("You are a helpful assistant.", "Hello!"));
        }
        static inline void stringToVoice(){}

        InputInterface<bool> wifiStatus_;
        static HTTPClient http_;
    };
}