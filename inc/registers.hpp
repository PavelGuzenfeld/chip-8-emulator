#ifndef REGISTERS_HPP
#define REGISTERS_HPP

#include"settings.hpp"

namespace chip8
{

struct Registers
{
    Vx m_vx;
    U16Bit m_index;
    U16Bit m_PC;
    U8Bit m_delayTimer;
    U8Bit m_soundTimer;
};

}   //namespace chip8

#endif  //REGISTERS_HPP