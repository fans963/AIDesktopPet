#pragma once

#include <SPI.h>
#include "gui/u8g2/font/src/u8g2_wqy.h"
#include "gui/u8g2/src/U8g2lib.h"
#include "core/component.hpp"

namespace gui
{
    using namespace core;
    class GUI:public Component
    {
    public:
        void beforeUpdate()override
        {
            u8g2.setFont(u8g2_font_wqy12_t_gb2312);
        }
        void update(){
            u8g2.sendBuffer();
        }

    private:
        U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI u8g2;
    };

} // namespace gui
