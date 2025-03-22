#pragma once

#include <cstddef>
#include <thread>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "core/component.hpp"
#include "globalDefine.hpp"
#include "log.hpp"

namespace network
{
    using namespace core;
    using namespace global;
    using namespace logger;

    static HTTPClient http_;
    class AI : public Component
    {
    public:
        void beforeUpdate() override
        {
            registerInput(wifiStatusId, wifiStatus_);
            registerInput(voiceReadyId, voiceReady_);
            registerInput(inputVoiceDataId, inputVoiceData_);
            registerInput(inputVoiceDataLengthId, inputVoiceDataLength_);
            registerOutput(aiReturnStringReadyId, aiReturnStringReady_);
            registerOutput(aiReturnStringId, aiReturnString_, "");
            registerOutput(outputVoiceDataReadyId, outputVoiceDataReady_, false);
            registerOutput(outputVoiceDataId, outputVoiceData_, nullptr);
            registerOutput(outputVoiceDataLengthId, outputVoiceDataLength_, 0);
        }
        void update() override
        {
            if (*wifiStatus_)
            {
                if (*voiceReady_)
                {
                    info("Start network request.");
                    if (networkRequestThread_.joinable())
                        networkRequestThread_.detach();
                }
            }
        }

    private:
        inline void getAccessToken()
        {
            const String url = String{baiduVoiceToStringTokenUrl} + "?grant_type=client_credentials" + "&client_id=" + baiduVoiceToStringApiKey + "&client_secret=" + baiduVoiceToStringSecretKey;
            http_.begin(url);

            uint8_t requestFrequency{0};
            do
            {
                const int responseFlag = http_.POST("");
                if (responseFlag == HTTP_CODE_OK)
                {
                    const String payload = http_.getString();
                    DynamicJsonDocument doc(512);
                    deserializeJson(doc, payload);

                    if (doc.containsKey("access_token"))
                    {
                        const char *token = doc["access_token"];
                        accessToken_ = token;
                        http_.end();
                        accessTokenGetted_ = true;
                        break;
                    }
                }
            } while (++requestFrequency < maxAiRequestFrequence);

            if (requestFrequency == maxAiRequestFrequence)
                error("The ai request frequence is more than the max request frequence %d", maxAiRequestFrequence);
        }

        static inline const char *jsonBodyForVoiceToString()
        {

        }
        static inline void voiceToString()
        {
            http_.begin(baiduVoiceToStringUrl);
            http_.addHeader("Content-Type", "application/json");
            http_.addHeader("Accept", "application/json");
            http_.addHeader("Authorization","Bearer");

            http_.end();
        }
        static inline const char *jsonBodyForDeepseek(const std::string &descriptor, const std::string &content)
        {
            const std::string jsonBody = "{\"model\":\"deepseek-reasoner\",\"messages\":[{\"role\":\"system\",\"content\":\"" + descriptor + "\"},{\"role\":\"user\",\"content\":\"" + content + "\"}],\"stream\":false}";
            return jsonBody.c_str();
        }
        static inline void ai()
        {
            http_.begin(deepseekApiUrl);
            http_.addHeader("Content-Type", "application/json");
            http_.addHeader("Authorization", deepseekApiKey);
            uint8_t requestFrequency{0};
            do
            {
                const auto resposeFlag = http_.POST(jsonBodyForDeepseek("You are a helpful assistant.", "Hello!"));
                if (resposeFlag == 200)
                {
                    const auto response = http_.getString();
                    http_.end();
                    break;
                }
            } while (++requestFrequency < maxAiRequestFrequence);
            if (requestFrequency == maxAiRequestFrequence)
                error("The ai request frequence is more than the max request frequence %d", maxAiRequestFrequence);
        }
        static inline void stringToVoice() {}

        InputInterface<bool> wifiStatus_;
        InputInterface<bool> voiceReady_;
        InputInterface<uint8_t *> inputVoiceData_;
        InputInterface<uint32_t> inputVoiceDataLength_;
        OutputInterface<bool> aiReturnStringReady_;
        OutputInterface<String> aiReturnString_;
        OutputInterface<bool> outputVoiceDataReady_;
        OutputInterface<uint8_t *> outputVoiceData_;
        OutputInterface<uint32_t> outputVoiceDataLength_;

        std::thread networkRequestThread_{[this]()
                                          {
                                              if (!accessTokenGetted_)
                                                  getAccessToken();
                                              voiceToString();
                                              ai();
                                              stringToVoice();
                                          }};
        std::atomic<bool> aiReturnStringStatus_{false};
        std::atomic<bool> outputVoiceDataStatus_{false};

        bool accessTokenGetted_{false};
        String accessToken_;
    };
}