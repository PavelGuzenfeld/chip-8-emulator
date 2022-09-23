#pragma once

#include <memory>
#include "settings.hpp"
#include "renderer.hpp"
#include "event_loop.hpp"
#include "canvas.hpp"
#include "code_reader.hpp"
#include "bus.hpp"
#include "instructionset.hpp"

namespace chip8
{

    class MainLoopRunner
    {
    public:
        MainLoopRunner(Bus &a_bus, Instructionset &a_instructionset, std::shared_ptr<EventLoop> const &a_loop);

        void run();

    private:
        Bus &m_bus;
        Instructionset &m_instructionset;
        std::shared_ptr<EventLoop> m_loop;
    };

} // namespace chip8
