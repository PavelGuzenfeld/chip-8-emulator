#pragma once

#include "settings.hpp"
#include "registers.hpp"
#include "keyboard.hpp"
#include "canvas.hpp"
#include "code_reader.hpp"
#include "instructionset.hpp"

namespace chip8
{

    class VirtualMachine
    {

    public:
        VirtualMachine(KeyBoard &a_keyBoard, Canvas &a_canvas, CodeRerader const &a_code);

        void delay();
        void beep();
        void execute();

    private:
        void loadCharacters();
        void loadCode(CodeRerader const &a_code);

    private:
        Memory m_memory;
        CallStack m_stack;
        Registers m_registers;
        InstructionSet m_instructionSet;
        KeyBoard &m_keyBoard;
        Canvas &m_canvas;
        Instructionset m_instructionset;
    };

} // namespace chip8
