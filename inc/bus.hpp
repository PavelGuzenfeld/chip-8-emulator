#pragma once

#include "registers.hpp"
#include "keyboard.hpp"
#include "canvas.hpp"

namespace chip8
{
    struct Bus
    {
        Memory &m_memory;
        CallStack &m_stack;
        Registers &m_registers;
        KeyBoard &m_keyBoard;
        Canvas &m_canvas;
    };

} // namespace chip8