#pragma once

#include "abstract.hpp"
#include "settings.hpp"

namespace chip8
{
    class EventLoop : public abc::Abstract
    {
    public:
        EventLoop() = default;

        virtual void run() = 0;
        virtual void close() = 0;
    };

} // namespace chip8