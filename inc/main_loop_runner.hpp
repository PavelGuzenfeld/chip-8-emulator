#pragma once

#include "settings.hpp"
#include "io.hpp"
#include "canvas.hpp"
#include "vm.hpp"
#include "code_reader.hpp"

namespace chip8
{

    class MainLoopRunner
    {
    public:
        MainLoopRunner(CodeRerader const &a_reader, StartupConfiguration const &a_config);

        void runMainLoop();

    private:
        Renderer m_renderer;
        Canvas m_canvas;
        KeyBoard m_keyBoard;
        EventLoop m_loop;
        VirtualMachine m_vm;
    };

} // namespace chip8
