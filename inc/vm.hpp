#ifndef VM_HPP
#define VM_HPP

//sleep()
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <algorithm>
#include "settings.hpp"
#include "registers.hpp"
#include "keyboard.hpp"
#include "canvas.hpp"
#include "code_reader.hpp"

namespace chip8
{

class VirtualMachine
{

public:
    VirtualMachine(KeyBoard& a_keyBoard, Canvas& a_canvas, CodeRerader const& a_code)
    :   m_memory{}
    ,   m_stack{}
    ,   m_registers{}
    ,   m_keyBoard{a_keyBoard}
    ,   m_canvas{a_canvas}
    {
        loadCharacters();
        loadCode(a_code);
    }

    void delay()
    {
        if(m_registers.m_delayTimer > 0)
        {
            sleep(m_DELAY_TIME);
            --m_registers.m_delayTimer;
        }
    }

    void beep()
    {
        if(m_registers.m_soundTimer > 0)
        {
            //TODO: Beep(15000, 100 * m_registers.m_soundTimer  );
            --m_registers.m_soundTimer;
        }
    }

    void exec()
    {
        auto instruction = readInstruction(m_registers.m_PC);
        std::cout << instruction << "\n";
        //TODO: execute
        m_registers.m_PC += 2;
    }

private:
    void loadCharacters()
    {
        m_memory = {
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
    }

    void loadCode(CodeRerader const& a_code)
    {
        std::copy(std::begin(a_code.buffer), 
            std::end(a_code.buffer), 
            m_memory.begin() +  CODE_LOAD_ADDRESS);
        m_registers.m_PC = CODE_LOAD_ADDRESS; 
    }

    U16Bit readInstruction(U16Bit a_address)
    {
        auto firstByte = m_memory[a_address];
        auto secondByte = m_memory[a_address + 1];
        return (firstByte << 8) | secondByte;
    }

private:
    Memory m_memory;
    CallStack m_stack;
    Registers m_registers;
    KeyBoard& m_keyBoard;
    Canvas& m_canvas;
    U16Bit const m_DELAY_TIME = 100;
    U16Bit const m_SOUND_FREQUENCY = 15'000;
    
};

}   //namespace chip8

#endif //VM_HPP