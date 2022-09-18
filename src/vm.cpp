// sleep()
#ifdef _WIN32
#include <Windows.h> Beep()
#else
#include <chrono> // std::chrono
#include <thread>
#include <unistd.h>  // sleep()
#include <iostream>  // std::cout
#include <exception> // std::exception
#endif
#include <algorithm> // copy()
#include "vm.hpp"

namespace chip8
{

    static auto const CHARS = Memory{
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80, // F
    };

    VirtualMachine::VirtualMachine(KeyBoard &a_keyBoard, Canvas &a_canvas, CodeRerader const &a_code)
        : m_memory{CHARS}, m_stack{}, m_registers{}, m_keyBoard{a_keyBoard}, m_canvas{a_canvas}, m_instructionset{{m_memory, m_stack, m_registers, m_keyBoard, m_canvas}}
    {
        loadCode(a_code);
    }

    void VirtualMachine::delay()
    {
        if (m_registers.m_delayTimer > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(REFRESH_RATE_MS));
            --m_registers.m_delayTimer;
        }
    }

    static void linuxBeep()
    {
        try
        {
            system("echo -e \"\007\" >/dev/tty10");
        }
        catch (std::exception const &)
        {
            std::cout << "\a"
                      << "second\n"
                      << std::flush;
        }
    }

    void VirtualMachine::beep()
    {
        if (m_registers.m_soundTimer > 0)
        {
#ifdef _WIN32
            Beep(REFRESH_RATE_MS, SOUND_FREQUENCY);
#else
            auto t_start = std::chrono::high_resolution_clock::now();
            while (true)
            {
                linuxBeep();
                auto t_end = std::chrono::high_resolution_clock::now();
                double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
                if (elapsed_time_ms > REFRESH_RATE_MS)
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
        auto opCode = m_instructionset.readInstruction(m_registers.m_PC);
#ifndef NDEBUG
        std::cout << "m_PC-> " << m_registers.m_PC << " opCode-> " << std::hex << std::showbase << opCode << "\n";
#endif
        m_registers.m_PC += 2;
        m_instructionset.runOpcode(opCode);
    }

    void VirtualMachine::drawSprite(U8Bit a_x, U8Bit a_y, U16Bit a_spriteAddress, U8Bit a_lines)
    {
        for (U8Bit offsetY = 0; offsetY < a_lines; ++offsetY)
        {
            auto y = a_y + offsetY;
            auto bitIndex = a_spriteAddress + offsetY;
            auto spriteBits = m_memory[bitIndex];
            auto vf = m_canvas.drawBits(a_x, y, spriteBits);
            m_registers.m_vx[0xf] = vf;
        }
    }

    void VirtualMachine::loadCode(CodeRerader const &a_code)
    {
        std::copy_n(std::begin(a_code.buffer), a_code.size,
                    m_memory.begin() + CODE_LOAD_ADDRESS);
        m_registers.m_PC = CODE_LOAD_ADDRESS;
    }

    U16Bit VirtualMachine::normalizeOpcode(U16Bit a_opCode)
    {
        // swap between first and 3'rd hex digit
        auto firstByte = (a_opCode & 0x000f) << 8;
        auto thirdByte = (a_opCode & 0x0f00) >> 8;
        auto others = a_opCode & 0xf0f0;
        return others | firstByte | thirdByte;
    }

} // namespace chip8
