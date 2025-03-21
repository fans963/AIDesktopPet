#pragma once

#include <SPI.h>
#include "gui/u8g2/font/src/u8g2_wqy.h"
#include "gui/u8g2/src/U8g2lib.h"
#include "core/component.hpp"
#include "util.hpp"

namespace gui
{
    using namespace core;
    class GUI : public Component
    {
    public:
        void beforeUpdate() override
        {
            u8g2.setFont(u8g2_font_wqy12_t_gb2312);
        }
        void update()
        {
            auto flag = util::createTimeOutTask([this]() -> bool {}, 10000000);
        }

    private:
        std::function<int(int)> drawString;
        U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI u8g2;
    };

} // namespace gui
