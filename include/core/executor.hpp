#pragma once

#include "core/component.hpp"
#include "network/wifi.hpp"
#include "voice/input.hpp"
#include "network/voice2string.hpp"
#include "network/ai.hpp"
#include "network/string2voice.hpp"
#include "voice/output.hpp"
#include "gui/gui.hpp"

namespace core
{
    class Executor
    {
    public:
        explicit Executor() {
            for (const auto &component : componentList_)
                component->beforeUpdate();
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
    };
} // namespace  core
