#pragma once

#include "core/component.hpp"
#include "network/wifi.hpp"
#include "voice/input.hpp"
#include "network/ai.hpp"
#include "voice/output.hpp"
#include "gui/gui.hpp"

namespace core
{
    using namespace network;
    class Executor
    {
    public:
        explicit Executor() {
            for (const auto &component : componentList_)
                component->beforeUpdate();

            //绑定组件数据地址，此处可以做自动依赖分析，但由于我们使用的是性能较差的MCU，且项目较小，为了榨干每一丝性能，我们进行一个人肉依赖分析（注意环形检测）。
            
        }
        ~Executor()
        {
            for (const auto &component : componentList_)
                if (component != nullptr)
                    delete component;
        }

        inline void update()
        {
            for (const auto &component : componentList_)
                component->update();
        }

    private:
        std::vector<Component *> componentList_;
        WIFI wifiComponent;
        AI aiComponent;
    };
} // namespace  core
