#ifndef VM_HPP
#define VM_HPP

//sleep()
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "settings.hpp"
#include "registers.hpp"
#include "keyboard.hpp"
#include "canvas.hpp"

namespace chip8
{

class VirtualMachine
{

public:
    VirtualMachine(KeyBoard& a_keyBoard, Canvas& a_canvas)
    :   m_memory{}
    ,   m_stack{}
    ,   m_registers{}
    ,   m_keyBoard{a_keyBoard}
    ,   m_canvas{a_canvas}
    {
        loadCharacters();
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