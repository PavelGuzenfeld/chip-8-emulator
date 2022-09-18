#pragma once

#include "settings.hpp"
#include "registers.hpp"
#include "keyboard.hpp"
#include "canvas.hpp"
#include "code_reader.hpp"

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

    class Instructionset
    {

    public:
        Instructionset(Bus const &a_bus);

        void delay();
        void beep();
        void drawSprite(U8Bit a_x, U8Bit a_y, U16Bit a_spriteAddress, U8Bit a_lines);
        void execute();
        void runOpcode(U16Bit a_opCode);
        U16Bit readInstruction(U16Bit a_address);

    private:
        void loadCharacters();
        void loadCode(CodeRerader const &a_code);

        auto instructionSetInit();
        U16Bit normalizeOpcode(U16Bit a_opCode);

    private:
        Bus m_bus;
        InstructionSet m_instructionSet;
    };

} // namespace chip8
