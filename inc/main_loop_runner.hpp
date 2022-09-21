#pragma once

#include "settings.hpp"
#include "io.hpp"
#include "canvas.hpp"
#include "vm.hpp"
#include "code_reader.hpp"
#include <memory>

namespace chip8
{

    class MainLoopRunner
    {
    public:
        MainLoopRunner(CodeRerader const &a_reader, std::shared_ptr<Renderer> a_renderer);

        void runMainLoop();

    private:
        std::shared_ptr<Renderer> m_renderer;
        Canvas m_canvas;
        KeyBoard m_keyBoard;
        EventLoop m_loop;
        VirtualMachine m_vm;
    };

} // namespace chip8
