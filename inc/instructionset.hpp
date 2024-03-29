#pragma once

#include "settings.hpp"
#include "bus.hpp"

namespace chip8
{
    class Instructionset
    {
    public:
        explicit Instructionset(Bus &a_bus);
        U16Bit readInstruction(U16Bit a_address);
        void runInstraction(U16Bit a_opCode);
        void runNextInstruction();

    private:
        auto instructionSetInit();
        void drawSprite(U8Bit a_x, U8Bit a_y, U16Bit a_spriteAddress, U8Bit a_lines);
        U16Bit normalizeOpcode(U16Bit a_opCode);

    private:
        Bus &m_bus;
        InstructionSet m_instructionSet;
    };

} // namespace chip8
