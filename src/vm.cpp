//sleep()
#ifdef _WIN32
#include <Windows.h> Beep()
#else
#include <unistd.h> // sleep()
#include <chrono> // std::chrono
#include <iostream> // std::cout
#endif
#include <algorithm> // copy()
#include "vm.hpp"

namespace chip8
{

static auto const CHARS = Memory{
        0xF0, 0x90, 0x90, 0x90, 0xF0, //0
        0xF0, 0x90, 0x90, 0x90, 0xF0, //1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
        0x90, 0x90, 0xF0, 0x10, 0x10, //4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
        0xF0, 0x80, 0xF0, 0x90, 0xF0, //7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
        0xF0, 0x90, 0xF0, 0x90, 0xF0, //9
        0xF0, 0x90, 0xF0, 0x90, 0x90, //A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
        0xF0, 0x80, 0x80, 0x80, 0xF0, //C
        0xE0, 0x90, 0x90, 0x90, 0xE0, //D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
        0xF0, 0x80, 0xF0, 0x80, 0x80, //F
    };

auto VirtualMachine::instructionSetInit()
{
    // initiates the instruction set
    return InstructionSet{

        // 00E0 - CLS
        {0x00e0, [&](U16Bit)
                    { 
                        // Clear the display.
                        m_canvas.clear();
                    }},

        //00EE - RET
        {0x00ee, [&](U16Bit)
                    {
                        // Return from a subroutine.
                        // Program counter is set to the address at the top of the stack.
                        m_registers.m_PC = m_stack.pop();
                    }},

        // 1NNN - JP addr
        {0x1fff, [&](U16Bit a_opCode)
                    {
                        // Jump to location NNN.
                        // The interpreter sets the program counter to NNN.
                        m_registers.m_PC = a_opCode & 0xfff;
                    }},

        // 2NNN - CALL addr
        {0x2fff, [&](U16Bit a_opCode)
                    {
                        // Call subroutine at NNN.
                        // The interpreter increments the stack pointer, 
                        //  then puts the current PC on the top of the stack.
                        // The PC is then set to NNN.
                        m_stack.push(m_registers.m_PC); 
                        m_registers.m_PC = a_opCode & 0xfff;
                    }},

        // 3XNN - SE Vx, NN
        {0x3fff, [&](U16Bit a_opCode)
                    {
                        // Skip next instruction if Vx = NN.
                        // The interpreter compares register Vx to NN, and if they are equal, 
                        // the interpreter increments the program counter by 2.
                        auto const reg = (a_opCode & 0xf00) >> 8;
                        auto const val = (a_opCode & 0x0ff);
                        if(m_registers.m_vx[reg] == val)
                        {
                            m_registers.m_PC += 2;
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
                        if(m_registers.m_vx[reg] != val)
                        {
                            m_registers.m_PC += 2;
                        }
                    }},

        // 5XY0 - SE Vx, Vy
        {0x5ff0, [&](U16Bit a_opCode)
                    {
                        // Skip next instruction if Vx = Vy.
                        // The interpreter compares register Vx to register Vy, and if they are equal,
                        // the interpreter increments the program counter by 2.
                        auto const regX = (a_opCode & 0xf00) >> 8;
                        auto const regY = (a_opCode & 0x0f0) >> 4;
                        if(m_registers.m_vx[regX] == m_registers.m_vx[regY])
                        {
                            m_registers.m_PC += 2;
                        }
                    }},

        // 6XNN - LD Vx, NN
        {0x6fff, [&](U16Bit a_opCode)
                    {
                        // Set Vx = NN.
                        // The interpreter puts the value NN into register Vx.
                        auto const reg = (a_opCode & 0xf00) >> 8;
                        auto const val = (a_opCode & 0x0ff);
                        m_registers.m_vx[reg] = val;
                    }},

        // 7XNN - ADD Vx, NN
        {0x7fff, [&](U16Bit a_opCode)
                    {
                        // Set Vx = Vx + NN.
                        // Adds the value NN to the value of Vx, then stores the result in Vx.
                        auto const reg = (a_opCode & 0xf00) >> 8;
                        auto const val = (a_opCode & 0x0ff);
                        m_registers.m_vx[reg] += val;
                    }},

        // 8XY0 - LD Vx, Vy
        {0x8ff0, [&](U16Bit a_opCode)
                    {
                        // Set Vx = Vy.
                        // Stores the value of Vy in Vx.
                        auto const regX = (a_opCode & 0xf00) >> 8;
                        auto const regY = (a_opCode & 0x0f0) >> 4;
                        m_registers.m_vx[regX] = m_registers.m_vx[regY];
                    }},

        // 8XY1 - OR Vx, Vy
        {0x8ff1, [&](U16Bit a_opCode)
                    {
                        // Set Vx = Vx OR Vy.
                        // Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx.
                        auto const regX = (a_opCode & 0xf00) >> 8;
                        auto const regY = (a_opCode & 0x0f0) >> 4;
                        m_registers.m_vx[regX] |= m_registers.m_vx[regY];
                    }},

        // 8XY2 - AND Vx, Vy
        {0x8ff2, [&](U16Bit a_opCode)
                    {
                        // Set Vx = Vx AND Vy.
                        // Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx.
                        auto const regX = (a_opCode & 0xf00) >> 8;
                        auto const regY = (a_opCode & 0x0f0) >> 4;
                        m_registers.m_vx[regX] &= m_registers.m_vx[regY];
                    }},

        // 8XY3 - XOR Vx, Vy
        {0x8ff3, [&](U16Bit a_opCode)
                    {
                        // Set Vx = Vx XOR Vy.
                        // Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx.
                        auto const regX = (a_opCode & 0xf00) >> 8;
                        auto const regY = (a_opCode & 0x0f0) >> 4;
                        m_registers.m_vx[regX] ^= m_registers.m_vx[regY];
                    }},

        // 8XY4 - ADD Vx, Vy
        {0x8ff4, [&](U16Bit a_opCode)
                    {
                        // Set Vx = Vx + Vy, set VF = carry.
                        // The values of Vx and Vy are added together.
                        // If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0.
                        // Only the lowest 8 bits of the result are kept, and stored in Vx.
                        auto const regX = (a_opCode & 0xf00) >> 8;
                        auto const regY = (a_opCode & 0x0f0) >> 4;
                        auto const result = m_registers.m_vx[regX] + m_registers.m_vx[regY];
                        m_registers.m_vx[regX] = result & 0xff;
                        m_registers.m_vx[0xf] = (result > 0xff) ? 1 : 0;
                    }},

        // 8XY5 - SUB Vx, Vy
        {0x8ff5, [&](U16Bit a_opCode)
                    {
                        // Set Vx = Vx - Vy, set VF = NOT borrow.
                        // If Vx > Vy, then VF is set to 1, otherwise 0.
                        // Then Vy is subtracted from Vx, and the results stored in Vx.
                        auto const regX = (a_opCode & 0xf00) >> 8;
                        auto const regY = (a_opCode & 0x0f0) >> 4;
                        auto const result = m_registers.m_vx[regX] - m_registers.m_vx[regY];
                        m_registers.m_vx[regX] = result & 0xff;
                        m_registers.m_vx[0xf] = (m_registers.m_vx[regX] > m_registers.m_vx[regY]) ? 1 : 0;
                    }},

        // 8XY6 - SHR Vx {, Vy}
        {0x8ff6, [&](U16Bit a_opCode)
                    {
                        // Set Vx = Vx SHR 1.
                        // If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0.
                        // Then Vx is divided by 2.
                        auto const regX = (a_opCode & 0xf00) >> 8;
                        m_registers.m_vx[0xf] = (m_registers.m_vx[regX] & 0x1) ? 1 : 0;
                        m_registers.m_vx[regX] >>= 1;
                    }},

        // 8XY7 - SUBN Vx, Vy
        {0x8ff7, [&](U16Bit a_opCode)
                    {
                        // Set Vx = Vy - Vx, set VF = NOT borrow.
                        // If Vy > Vx, then VF is set to 1, otherwise 0.
                        // Then Vx is subtracted from Vy, and the results stored in Vx.
                        auto const regX = (a_opCode & 0xf00) >> 8;
                        auto const regY = (a_opCode & 0x0f0) >> 4;
                        auto const result = m_registers.m_vx[regY] - m_registers.m_vx[regX];
                        m_registers.m_vx[regX] = result & 0xff;
                        m_registers.m_vx[0xf] = (m_registers.m_vx[regY] > m_registers.m_vx[regX]) ? 1 : 0;
                    }},

        // 8XYE - SHL Vx {, Vy}
        {0x8ffe, [&](U16Bit a_opCode)
                    {
                        // Set Vx = Vx SHL 1.
                        // If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0.
                        // Then Vx is multiplied by 2.
                        auto const regX = (a_opCode & 0xf00) >> 8;
                        m_registers.m_vx[0xf] = (m_registers.m_vx[regX] & 0x80) ? 1 : 0;
                        m_registers.m_vx[regX] <<= 1;
                    }},

        // 9XY0 - SNE Vx, Vy
        {0x9ff0, [&](U16Bit a_opCode)
                    {
                        // Skip next instruction if Vx != Vy.
                        // The values of Vx and Vy are compared, and if they are not equal,
                        // the program counter is increased by 2.
                        auto const regX = (a_opCode & 0xf00) >> 8;
                        auto const regY = (a_opCode & 0x0f0) >> 4;
                        if (m_registers.m_vx[regX] != m_registers.m_vx[regY])
                        {
                            m_registers.m_PC += 2;
                        }
                    }},

        // ANNN - LD I, addr
        {0xafff, [&](U16Bit a_opCode)
                    {
                        // Set I = nnn.
                        // The value of register I is set to nnn.
                        m_registers.m_index = a_opCode & 0x0fff;
                    }},
        
        // BNNN - JP V0, addr
        {0xbfff, [&](U16Bit a_opCode)
                    {
                        // Jump to location nnn + V0.
                        // The program counter is set to nnn plus the value of V0.
                        m_registers.m_PC = (a_opCode & 0x0fff) + m_registers.m_vx[0];
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
                        m_registers.m_vx[regX] = (rand() % 0x100) & kk;
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
                        auto const x = m_registers.m_vx[regX];
                        auto const y = m_registers.m_vx[regY];
                        m_registers.m_vx[0xf] = 0;
                        drawSprite(x, y, m_registers.m_index, n);
                    }},

        // EX9E - SKP Vx
        {0xef9e, [&](U16Bit a_opCode)
                    {
                        // Skip next instruction if key with the value of Vx is pressed.
                        // Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.
                        auto const regX = (a_opCode & 0xf00) >> 8;
                        auto const key = m_registers.m_vx[regX];
                        if (m_keyBoard.isVirtualKeyDown(key))
                        {
                            m_registers.m_PC += 2;
                        }
                    }},

        // EXA1 - SKNP Vx
        {0xefa1, [&](U16Bit a_opCode)
                    {
                        // Skip next instruction if key with the value of Vx is not pressed.
                        // Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2.
                        auto const regX = (a_opCode & 0xf00) >> 8;
                        auto const key = m_registers.m_vx[regX];
                        if (!m_keyBoard.isVirtualKeyDown(key))
                        {
                            m_registers.m_PC += 2;
                        }
                    }},

        // FX07 - LD Vx, DT
        {0xff07, [&](U16Bit a_opCode)
                    {
                        // Set Vx = delay timer value.
                        // The value of DT is placed into Vx.
                        auto const regX = (a_opCode & 0xf00) >> 8;
                        m_registers.m_vx[regX] = m_registers.m_delayTimer;
                    }},
        
        // FX0A - LD Vx, K
        {0xff0a, [&](U16Bit a_opCode)
                    {
                        // Wait for a key press, store the value of the key in Vx.
                        // All execution stops until a key is pressed, then the value of that key is stored in Vx.
                        auto const regX = (a_opCode & 0xf00) >> 8;
                        m_registers.m_vx[regX] = m_keyBoard.waitForKeyPress();
                    }},

        // FX15 - LD DT, Vx
        {0xff15, [&](U16Bit a_opCode)
                    {
                        // Set delay timer = Vx.
                        // DT is set equal to the value of Vx.
                        auto const regX = (a_opCode & 0xf00) >> 8;
                        m_registers.m_delayTimer = m_registers.m_vx[regX];
                    }},

        // FX18 - LD ST, Vx
        {0xff18, [&](U16Bit a_opCode)
                    {
                        // Set sound timer = Vx.
                        // ST is set equal to the value of Vx.
                        auto const regX = (a_opCode & 0xf00) >> 8;
                        m_registers.m_soundTimer = m_registers.m_vx[regX];
                    }},
        
        // FX1E - ADD I, Vx
        {0xff1e, [&](U16Bit a_opCode)
                    {
                        // Set I = I + Vx.
                        // The values of I and Vx are added, and the results are stored in I.
                        auto const regX = (a_opCode & 0xf00) >> 8;
                        m_registers.m_index += m_registers.m_vx[regX];
                    }},

        // FX29 - LD F, Vx
        {0xff29, [&](U16Bit a_opCode)
                    {
                        // Set I = location of sprite for digit Vx.
                        // The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx.
                        auto const regX = (a_opCode & 0xf00) >> 8;
                        m_registers.m_index = m_registers.m_vx[regX] * 5;
                    }},

        // FX33 - LD B, Vx
        {0xff33, [&](U16Bit a_opCode)
                    {
                        // Store BCD representation of Vx in memory locations I, I+1, and I+2.
                        // The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.
                        auto const regX = (a_opCode & 0xf00) >> 8;
                        auto const value = m_registers.m_vx[regX];
                        m_memory[m_registers.m_index] =  value / 100;
                        m_memory[m_registers.m_index + 1] = (value / 10) % 10;
                        m_memory[m_registers.m_index + 2] = value % 10;
                    }},

        // FX55 - LD [I], Vx
        {0xff55, [&](U16Bit a_opCode)
                    {
                        // Store registers V0 through Vx in memory starting at location I.
                        // The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.
                        auto const regX = (a_opCode & 0xf00) >> 8;
                        for (auto i = 0; i <= regX; ++i)
                        {
                            m_memory[m_registers.m_index + i] = m_registers.m_vx[i];
                        }
                    }},

        // FX65 - LD Vx, [I]
        {0xff65, [&](U16Bit a_opCode)
                    {
                        // Read registers V0 through Vx from memory starting at location I.
                        // The interpreter reads values from memory starting at location I into registers V0 through Vx.
                        auto const regX = (a_opCode & 0xf00) >> 8;
                        for (auto i = 0; i <= regX; ++i)
                        {
                            m_registers.m_vx[i] = m_memory[m_registers.m_index + i];
                        }
                    }},
        
        //   0x0000, [&](U16Bit a_opCode)
        //   {  
        //       // Noop
        //   }},

    };
}

VirtualMachine::VirtualMachine(KeyBoard& a_keyBoard, Canvas& a_canvas, CodeRerader const& a_code)
:   m_memory{CHARS}
,   m_stack{}
,   m_registers{}
,   m_instructionSet{instructionSetInit()}
,   m_keyBoard{a_keyBoard}
,   m_canvas{a_canvas}
{
    loadCode(a_code);
}

void VirtualMachine::delay()
{
    if(m_registers.m_delayTimer > 0)
    {
        sleep(m_DELAY_TIME);
        --m_registers.m_delayTimer;
    }
}

static void linuxBeep()
{
    system("echo -e \"\007\" >/dev/tty10");
    std::cout << "\a" << "second\n" << std::flush;
}

void VirtualMachine::beep()
{
    if(m_registers.m_soundTimer > 0)
    {
        #ifdef _WIN32
            Beep(m_SOUND_FREQUENCY, m_DELAY_TIME);
        #else
            auto t_start = std::chrono::high_resolution_clock::now();
            while(true)
            {
                linuxBeep();
                auto t_end = std::chrono::high_resolution_clock::now();
                double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end-t_start).count();
                if(elapsed_time_ms < m_DELAY_TIME)
                {
                    break;
                }
            }
        #endif
        --m_registers.m_soundTimer;
    }
}

void VirtualMachine::execute()
{
    auto opCode = readInstruction(m_registers.m_PC);
    std::cout << "m_PC-> " << m_registers.m_PC << "opCode-> " << opCode << "\n";
    m_registers.m_PC += 2;
    m_instructionSet.lower_bound(opCode)->second(opCode);
    
}

void VirtualMachine::drawSprite(U8Bit a_x, U8Bit a_y, U16Bit a_spriteAddress, U8Bit a_lines)
{
    for(U8Bit offsetY = 0; offsetY < a_lines; ++offsetY)
    {
        auto y = a_y + offsetY;
        auto vf = m_canvas.drawBits(a_x, y, m_memory[a_spriteAddress + y]);
        if(vf == 1)
        {
            m_registers.m_vx[0xf] = vf;
        }
    }
}

void VirtualMachine::loadCode(CodeRerader const& a_code)
{
    std::copy_n(std::begin(a_code.buffer), a_code.size,
        m_memory.begin() +  CODE_LOAD_ADDRESS);
    m_registers.m_PC = CODE_LOAD_ADDRESS; 
}

U16Bit VirtualMachine::readInstruction(U16Bit a_address)
{
    auto firstByte = m_memory[a_address];
    auto secondByte = m_memory[a_address + 1];
    return (firstByte << 8) | secondByte;
}



}   //namespace chip8
