#pragma once

#ifdef DEBUG

#include <chrono>
#include <sstream>
#include <iomanip>
#include <HardwareSerial.h>
#include <esp_system.h>

namespace logger
{
    static inline void info(const char *msg,...)
    {
        const auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::stringstream timeStream;
        timeStream << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S");
        Serial.print(timeStream.str().c_str());
        Serial.print("[INFO] ");
        Serial.println(msg);
    }

    static inline void warn(const char *msg, ...)
    {
        const auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::stringstream timeStream;
        timeStream << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S");
        Serial.print(timeStream.str().c_str());
        Serial.print("[WARN] ");
        Serial.println(msg);
    }

    static inline void error(const char *msg, ...)
    {
        const auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::stringstream timeStream;
        timeStream << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S");
        Serial.print(timeStream.str().c_str());
        Serial.print("[error] ");
        Serial.println(msg);
        esp_restart();
    }
} // namespace log
#endif