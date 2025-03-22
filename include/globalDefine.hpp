#pragma once
#include <string>
#include "util.hpp"

namespace global
{
    static inline constexpr char *wifiSsid{"test"};
    static inline constexpr char *wifiPassword{"test"};

    static inline constexpr char *deepseekApiUrl{"test"};
    static inline constexpr char *deepseekApiKey{"test"};

    static inline constexpr char *baiduVoiceToStringTokenUrl{"https://aip.baidubce.com/oauth/2.0/token"};
    static inline constexpr char *baiduVoiceToStringUrl{"https://vop.baidu.com/pro_api"};
    static inline constexpr char *baiduVoiceToStringApiKey{"VHv9E2YSIP7ga6YmJoHra43H"};
    static inline constexpr char *baiduVoiceToStringSecretKey{"WQ5HDb7QRdXyYwIxYo7MRdOlHWyUIED3"};
    static inline constexpr char *baiduStringToVoiceUrl{"test"};
    static inline constexpr char *baiduStringToVoiceKey{"test"};

    static inline constexpr uint8_t maxAiRequestFrequence{5};

    static inline constexpr auto wifiStatusId = util::next();
    static inline constexpr auto voiceReadyId = util::next();
    static inline constexpr auto inputVoiceDataId = util::next();
    static inline constexpr auto inputVoiceDataLengthId = util::next();
    static inline constexpr auto aiReturnStringReadyId = util::next();
    static inline constexpr auto aiReturnStringId = util::next();
    static inline constexpr auto outputVoiceDataReadyId = util::next();
    static inline constexpr auto outputVoiceDataId = util::next();
    static inline constexpr auto outputVoiceDataLengthId = util::next();
}