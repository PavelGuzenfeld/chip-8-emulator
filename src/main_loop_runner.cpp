#include "main_loop_runner.hpp"
#include "delay_functions.hpp"
namespace chip8
{

    MainLoopRunner::MainLoopRunner(Bus &a_bus, Instructionset &a_instructionset, std::shared_ptr<EventLoop> const &a_loop)
        : m_bus{a_bus},
          m_instructionset{a_instructionset},
          m_loop{a_loop}
    {
    }

    void MainLoopRunner::run()
    {
        while (true)
        {
            m_loop->run();
            m_instructionset.runNextInstruction();
            delay(m_bus.m_registers.m_delayTimer);
            beep(m_bus.m_registers.m_soundTimer);
        }
    }

} // namespace chip8