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

VirtualMachine::VirtualMachine(KeyBoard& a_keyBoard, Canvas& a_canvas, CodeRerader const& a_code)
:   m_memory{CHARS}
,   m_stack{}
,   m_registers{}
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

void VirtualMachine::exec()
{
    auto instruction = readInstruction(m_registers.m_PC);
    std::cout << instruction << "\n";
    //TODO: execute
    m_registers.m_PC += 2;
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
