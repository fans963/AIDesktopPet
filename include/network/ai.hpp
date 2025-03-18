#pragma once

#include <thread>
#include "core/component.hpp"

namespace network{
    using namespace core;
    class AI : public Component
    {
        public:
        void beforeUpdate()override{}
        void update() override{
            auto aiThread = std::thread{[this](){}};
        }

        private:

    };
}