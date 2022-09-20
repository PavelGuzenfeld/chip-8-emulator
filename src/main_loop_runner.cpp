#include "main_loop_runner.hpp"

namespace chip8
{

    MainLoopRunner::MainLoopRunner(CodeRerader const &a_reader, StartupConfiguration const &a_config)
        : m_renderer{a_config}, m_canvas{m_renderer, CANVAS_WIDTH, CANVAS_HEIGHT}, m_keyBoard{KEY_BOARD}, m_loop{onKeyDown(m_keyBoard), onKeyUp(m_keyBoard)}, m_vm{m_keyBoard, m_canvas, a_reader}
    {
    }

    void MainLoopRunner::runMainLoop()
    {
        while (true)
        {
            m_loop.run();
            m_vm.execute();
            m_vm.delay();
            m_vm.beep();
        }
    }

} // namespace chip8