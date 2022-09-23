// sleep()
#ifdef _WIN32
#include <Windows.h> Beep()
#else
#include <chrono> // std::chrono
#include <thread>
#include <unistd.h> // sleep()
#include <iostream> // std::cout
#endif
#include <algorithm> // copy()
#include "instructionset.hpp"

namespace chip8
{

    auto Instructionset::instructionSetInit()
    {
        // initiates the instruction set
        return InstructionSet{

            // 00E0 - CLS
            {0x00e0, [&](U16Bit)
             {
                 // Clear the display.
                 m_bus.m_canvas.clear();
             }},

            // 00EE - RET
            {0x0ee0, [&](U16Bit)
             {
                 // Return from a subroutine.
                 // Program counter is set to the address at the top of the stack.
                 m_bus.m_registers.m_PC = m_bus.m_stack.pop();
             }},

            // 1NNN - JP addr
            {0x1fff, [&](U16Bit a_opCode)
             {
                 // Jump to location NNN.
                 // The interpreter sets the program counter to NNN.
                 m_bus.m_registers.m_PC = a_opCode & 0x0fff;
             }},

            // 2NNN - CALL addr
            {0x2fff, [&](U16Bit a_opCode)
             {
                 // Call subroutine at NNN.
                 // The interpreter increments the stack pointer,
                 //  then puts the current PC on the top of the stack.
                 // The PC is then set to NNN.
                 m_bus.m_stack.push(m_bus.m_registers.m_PC);
                 m_bus.m_registers.m_PC = a_opCode & 0x0fff;
             }},

            // 3XNN - SE Vx, NN
            {0x3fff, [&](U16Bit a_opCode)
             {
                 // Skip next instruction if Vx = NN.
                 // The interpreter compares register Vx to NN, and if they are equal,
                 // the interpreter increments the program counter by 2.
                 auto const reg = (a_opCode & 0x0f00) >> 8;
                 auto const val = (a_opCode & 0x00ff);
                 if (m_bus.m_registers.m_vx[reg] == val)
                 {
                     m_bus.m_registers.m_PC += 2;
                 }
             }},

            // 4XNN - SNE Vx, NN
            {0x4fff, [&](U16Bit a_opCode)
             {
                 // Skip next instruction if Vx != NN.
                 // The interpreter compares register Vx to NN, and if they are not equal,
                 // the interpreter increments the program counter by 2.
                 auto const reg = (a_opCode & 0xf00) >> 8;
                 auto const val = (a_opCode & 0x0ff);
                 if (m_bus.m_registers.m_vx[reg] != val)
                 {
                     m_bus.m_registers.m_PC += 2;
                 }
             }},

            // 5XY0 - SE Vx, Vy
            {0x50ff, [&](U16Bit a_opCode)
             {
                 // Skip next instruction if Vx = Vy.
                 // The interpreter compares register Vx to register Vy, and if they are equal,
                 // the interpreter increments the program counter by 2.
                 auto const regX = (a_opCode & 0xf00) >> 8;
                 auto const regY = (a_opCode & 0x0f0) >> 4;
                 if (m_bus.m_registers.m_vx[regX] == m_bus.m_registers.m_vx[regY])
                 {
                     m_bus.m_registers.m_PC += 2;
                 }
             }},

            // 6XNN - LD Vx, NN
            {0x6fff, [&](U16Bit a_opCode)
             {
                 // Set Vx = NN.
                 // The interpreter puts the value NN into register Vx.
                 auto const reg = (a_opCode & 0xf00) >> 8;
                 auto const val = (a_opCode & 0x0ff);
                 m_bus.m_registers.m_vx[reg] = val;
             }},

            // 7XNN - ADD Vx, NN
            {0x7fff, [&](U16Bit a_opCode)
             {
                 // Set Vx = Vx + NN.
                 // Adds the value NN to the value of Vx, then stores the result in Vx.
                 auto const reg = (a_opCode & 0xf00) >> 8;
                 auto const val = (a_opCode & 0x0ff);
                 m_bus.m_registers.m_vx[reg] += val;
             }},

            // 8XY0 - LD Vx, Vy
            {0x80ff, [&](U16Bit a_opCode)
             {
                 // Set Vx = Vy.
                 // Stores the value of Vy in Vx.
                 auto const regX = (a_opCode & 0xf00) >> 8;
                 auto const regY = (a_opCode & 0x0f0) >> 4;
                 m_bus.m_registers.m_vx[regX] = m_bus.m_registers.m_vx[regY];
             }},

            // 8XY1 - OR Vx, Vy
            {0x81ff, [&](U16Bit a_opCode)
             {
                 // Set Vx = Vx OR Vy.
                 // Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx.
                 auto const regX = (a_opCode & 0xf00) >> 8;
                 auto const regY = (a_opCode & 0x0f0) >> 4;
                 m_bus.m_registers.m_vx[regX] |= m_bus.m_registers.m_vx[regY];
             }},

            // 8XY2 - AND Vx, Vy
            {0x82ff, [&](U16Bit a_opCode)
             {
                 // Set Vx = Vx AND Vy.
                 // Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx.
                 auto const regX = (a_opCode & 0xf00) >> 8;
                 auto const regY = (a_opCode & 0x0f0) >> 4;
                 m_bus.m_registers.m_vx[regX] &= m_bus.m_registers.m_vx[regY];
             }},

            // 8XY3 - XOR Vx, Vy
            {0x83ff, [&](U16Bit a_opCode)
             {
                 // Set Vx = Vx XOR Vy.
                 // Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx.
                 auto const regX = (a_opCode & 0xf00) >> 8;
                 auto const regY = (a_opCode & 0x0f0) >> 4;
                 m_bus.m_registers.m_vx[regX] ^= m_bus.m_registers.m_vx[regY];
             }},

            // 8XY4 - ADD Vx, Vy
            {0x84ff, [&](U16Bit a_opCode)
             {
                 // Set Vx = Vx + Vy, set VF = carry.
                 // The values of Vx and Vy are added together.
                 // If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0.
                 // Only the lowest 8 bits of the result are kept, and stored in Vx.
                 auto const regX = (a_opCode & 0xf00) >> 8;
                 auto const regY = (a_opCode & 0x0f0) >> 4;
                 auto const result = m_bus.m_registers.m_vx[regX] + m_bus.m_registers.m_vx[regY];
                 m_bus.m_registers.m_vx[regX] = result & 0xff;
                 m_bus.m_registers.m_vx[0xf] = (result > 0xff) ? 1 : 0;
             }},

            // 8XY5 - SUB Vx, Vy
            {0x85ff, [&](U16Bit a_opCode)
             {
                 // Set Vx = Vx - Vy, set VF = NOT borrow.
                 // If Vx > Vy, then VF is set to 1, otherwise 0.
                 // Then Vy is subtracted from Vx, and the results stored in Vx.
                 auto const regX = (a_opCode & 0xf00) >> 8;
                 auto const regY = (a_opCode & 0x0f0) >> 4;
                 auto const result = m_bus.m_registers.m_vx[regX] - m_bus.m_registers.m_vx[regY];
                 m_bus.m_registers.m_vx[regX] = result & 0xff;
                 m_bus.m_registers.m_vx[0xf] = (m_bus.m_registers.m_vx[regX] > m_bus.m_registers.m_vx[regY]) ? 1 : 0;
             }},

            // 8XY6 - SHR Vx {, Vy}
            {0x86ff, [&](U16Bit a_opCode)
             {
                 // Set Vx = Vx SHR 1.
                 // If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0.
                 // Then Vx is divided by 2.
                 auto const regX = (a_opCode & 0xf00) >> 8;
                 m_bus.m_registers.m_vx[0xf] = (m_bus.m_registers.m_vx[regX] & 0x1) ? 1 : 0;
                 m_bus.m_registers.m_vx[regX] >>= 1;
             }},

            // 8XY7 - SUBN Vx, Vy
            {0x87ff, [&](U16Bit a_opCode)
             {
                 // Set Vx = Vy - Vx, set VF = NOT borrow.
                 // If Vy > Vx, then VF is set to 1, otherwise 0.
                 // Then Vx is subtracted from Vy, and the results stored in Vx.
                 auto const regX = (a_opCode & 0xf00) >> 8;
                 auto const regY = (a_opCode & 0x0f0) >> 4;
                 auto const result = m_bus.m_registers.m_vx[regY] - m_bus.m_registers.m_vx[regX];
                 m_bus.m_registers.m_vx[regX] = result & 0xff;
                 m_bus.m_registers.m_vx[0xf] = (m_bus.m_registers.m_vx[regY] > m_bus.m_registers.m_vx[regX]) ? 1 : 0;
             }},

            // 8XYE - SHL Vx {, Vy}
            {0x8eff, [&](U16Bit a_opCode)
             {
                 // Set Vx = Vx SHL 1.
                 // If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0.
                 // Then Vx is multiplied by 2.
                 auto const regX = (a_opCode & 0xf00) >> 8;
                 m_bus.m_registers.m_vx[0xf] = (m_bus.m_registers.m_vx[regX] & 0x80) ? 1 : 0;
                 m_bus.m_registers.m_vx[regX] <<= 1;
             }},

            // 9XY0 - SNE Vx, Vy
            {0x90ff, [&](U16Bit a_opCode)
             {
                 // Skip next instruction if Vx != Vy.
                 // The values of Vx and Vy are compared, and if they are not equal,
                 // the program counter is increased by 2.
                 auto const regX = (a_opCode & 0xf00) >> 8;
                 auto const regY = (a_opCode & 0x0f0) >> 4;
                 if (m_bus.m_registers.m_vx[regX] != m_bus.m_registers.m_vx[regY])
                 {
                     m_bus.m_registers.m_PC += 2;
                 }
             }},

            // ANNN - LD I, addr
            {0xafff, [&](U16Bit a_opCode)
             {
                 // Set I = nnn.
                 // The value of register I is set to nnn.
                 m_bus.m_registers.m_index = a_opCode & 0x0fff;
             }},

            // BNNN - JP V0, addr
            {0xbfff, [&](U16Bit a_opCode)
             {
                 // Jump to location nnn + V0.
                 // The program counter is set to nnn plus the value of V0.
                 m_bus.m_registers.m_PC = (a_opCode & 0x0fff) + m_bus.m_registers.m_vx[0];
             }},

            // CXNN - RND Vx, byte
            {0xcfff, [&](U16Bit a_opCode)
             {
                 // Set Vx = random byte AND kk.
                 // The interpreter generates a random number from 0 to 255,
                 // which is then ANDed with the value kk.
                 // The results are stored in Vx.
                 auto const regX = (a_opCode & 0xf00) >> 8;
                 auto const kk = a_opCode & 0x00ff;
                 m_bus.m_registers.m_vx[regX] = (rand() % 0x100) & kk;
             }},

            // DXYN - DRW Vx, Vy, nibble
            {0xdfff, [&](U16Bit a_opCode)
             {
                 // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
                 // The interpreter reads n bytes from memory, starting at the address stored in I.
                 // These bytes are then displayed as sprites on screen at coordinates (Vx, Vy).
                 // Sprites are XORed onto the existing screen.
                 // If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0.
                 // If the sprite is positioned so part of it is outside the drawing area, it wraps around to the opposite side of the screen.
                 auto const regX = (a_opCode & 0xf00) >> 8;
                 auto const regY = (a_opCode & 0x0f0) >> 4;
                 auto const n = a_opCode & 0x000f;
                 auto const x = m_bus.m_registers.m_vx[regX];
                 auto const y = m_bus.m_registers.m_vx[regY];
                 drawSprite(x, y, m_bus.m_registers.m_index, n);
             }},

            // EX9E - SKP Vx
            {0xee9f, [&](U16Bit a_opCode)
             {
                 // Skip next instruction if key with the value of Vx is pressed.
                 // Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.
                 auto const regX = (a_opCode & 0xf00) >> 8;
                 auto const key = m_bus.m_registers.m_vx[regX];
                 if (m_bus.m_keyBoard.isVirtualKeyDown(key))
                 {
                     m_bus.m_registers.m_PC += 2;
                 }
             }},

            // EXA1 - SKNP Vx
            {0xe1af, [&](U16Bit a_opCode)
             {
                 // Skip next instruction if key with the value of Vx is not pressed.
                 // Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2.
                 auto const regX = (a_opCode & 0xf00) >> 8;
                 auto const key = m_bus.m_registers.m_vx[regX];
                 if (!m_bus.m_keyBoard.isVirtualKeyDown(key))
                 {
                     m_bus.m_registers.m_PC += 2;
                 }
             }},

            // FX07 - LD Vx, DT
            {0xf70f, [&](U16Bit a_opCode)
             {
                 // Set Vx = delay timer value.
                 // The value of DT is placed into Vx.
                 auto const regX = (a_opCode & 0xf00) >> 8;
                 m_bus.m_registers.m_vx[regX] = m_bus.m_registers.m_delayTimer;
             }},

            // FX0A - LD Vx, K
            {0xfa0f, [&](U16Bit a_opCode)
             {
                 // Wait for a key press, store the value of the key in Vx.
                 // All execution stops until a key is pressed, then the value of that key is stored in Vx.
                 auto const regX = (a_opCode & 0xf00) >> 8;
                 m_bus.m_registers.m_vx[regX] = m_bus.m_keyBoard.waitForKeyPress();
             }},

            // FX15 - LD DT, Vx
            {0xf51f, [&](U16Bit a_opCode)
             {
                 // Set delay timer = Vx.
                 // DT is set equal to the value of Vx.
                 auto const regX = (a_opCode & 0xf00) >> 8;
                 m_bus.m_registers.m_delayTimer = m_bus.m_registers.m_vx[regX];
             }},

            // FX18 - LD ST, Vx
            {0xf81f, [&](U16Bit a_opCode)
             {
                 // Set sound timer = Vx.
                 // ST is set equal to the value of Vx.
                 auto const regX = (a_opCode & 0xf00) >> 8;
                 m_bus.m_registers.m_soundTimer = m_bus.m_registers.m_vx[regX];
             }},

            // FX1E - ADD I, Vx
            {0xfe1f, [&](U16Bit a_opCode)
             {
                 // Set I = I + Vx.
                 // The values of I and Vx are added, and the results are stored in I.
                 auto const regX = (a_opCode & 0xf00) >> 8;
                 m_bus.m_registers.m_index += m_bus.m_registers.m_vx[regX];
             }},

            // FX29 - LD F, Vx
            {0xf92f, [&](U16Bit a_opCode)
             {
                 // Set I = location of sprite for digit Vx.
                 // The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx.
                 auto const regX = (a_opCode & 0xf00) >> 8;
                 m_bus.m_registers.m_index = m_bus.m_registers.m_vx[regX] * 5;
             }},

            // FX33 - LD B, Vx
            {0xf33f, [&](U16Bit a_opCode)
             {
                 // Store BCD representation of Vx in memory locations I, I+1, and I+2.
                 // The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.
                 auto const regX = (a_opCode & 0xf00) >> 8;
                 auto const value = m_bus.m_registers.m_vx[regX];
                 m_bus.m_memory[m_bus.m_registers.m_index] = value / 100;
                 m_bus.m_memory[m_bus.m_registers.m_index + 1] = (value / 10) % 10;
                 m_bus.m_memory[m_bus.m_registers.m_index + 2] = value % 10;
             }},

            // FX55 - LD [I], Vx
            {0xf55f, [&](U16Bit a_opCode)
             {
                 // Store registers V0 through Vx in memory starting at location I.
                 // The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.
                 auto const regX = (a_opCode & 0xf00) >> 8;
                 for (auto i = 0; i <= regX; ++i)
                 {
                     m_bus.m_memory[m_bus.m_registers.m_index + i] = m_bus.m_registers.m_vx[i];
                 }
             }},

            // FX65 - LD Vx, [I]
            {0xf56f, [&](U16Bit a_opCode)
             {
                 // Read registers V0 through Vx from memory starting at location I.
                 // The interpreter reads values from memory starting at location I into registers V0 through Vx.
                 auto const regX = (a_opCode & 0xf00) >> 8;
                 for (auto i = 0; i <= regX; ++i)
                 {
                     m_bus.m_registers.m_vx[i] = m_bus.m_memory[m_bus.m_registers.m_index + i];
                 }
             }},

            //   0x0000, [&](U16Bit a_opCode)
            //   {
            //       // Noop
            //   }},

        };
    }

    Instructionset::Instructionset(Bus &a_bus)
        : m_bus{a_bus}, m_instructionSet{instructionSetInit()}
    {
    }

    void Instructionset::drawSprite(U8Bit a_x, U8Bit a_y, U16Bit a_spriteAddress, U8Bit a_lines)
    {
        for (U8Bit offsetY = 0; offsetY < a_lines; ++offsetY)
        {
            auto y = a_y + offsetY;
            auto bitIndex = a_spriteAddress + offsetY;
            auto spriteBits = m_bus.m_memory[bitIndex];
            auto vf = m_bus.m_canvas.drawBits(a_x, y, spriteBits);
            m_bus.m_registers.m_vx[0xf] = vf;
        }
    }

    U16Bit Instructionset::readInstruction(U16Bit a_address)
    {
        auto firstByte = m_bus.m_memory[a_address];
        auto secondByte = m_bus.m_memory[a_address + 1];
        return (firstByte << 8) | secondByte;
    }

    U16Bit Instructionset::normalizeOpcode(U16Bit a_opCode)
    {
        // swap between first and 3'rd hex digit
        auto firstByte = (a_opCode & 0x000f) << 8;
        auto thirdByte = (a_opCode & 0x0f00) >> 8;
        auto others = a_opCode & 0xf0f0;
        return others | firstByte | thirdByte;
    }

    void Instructionset::runInstraction(U16Bit a_opCode)
    {
        auto normalizedOpcode = normalizeOpcode(a_opCode);
        auto runCommand = m_instructionSet.lower_bound(normalizedOpcode)->second;
        runCommand(a_opCode);
    }

    void Instructionset::runNextInstruction()
    {
        auto opCode = readInstruction(m_bus.m_registers.m_PC);
#ifndef NDEBUG
        std::cout << "m_PC-> " << m_bus.m_registers.m_PC << " opCode-> " << std::hex << std::showbase << opCode << "\n";
#endif
        m_bus.m_registers.m_PC += 2;
        runInstraction(opCode);
    }

} // namespace chip8
