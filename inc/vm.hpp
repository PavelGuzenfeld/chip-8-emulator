#ifndef VM_HPP
#define VM_HPP

#include "settings.hpp"

namespace chip8
{

class vm
{
    Stack<u_short, STACK_CAPACITY> m_stack;
    Registers m_vRegisters;
    Memory m_memory;
    u_short m_index;

    u_char m_delayTimer;
    u_char m_soundTimer;

    u_short m_PC;
    
};

}   //namespace chip8

#endif //VM_HPP