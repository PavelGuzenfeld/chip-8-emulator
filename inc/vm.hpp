#ifndef VM_HPP
#define VM_HPP

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
    VirtualMachine(KeyBoard& a_keyBoard, Canvas& a_canvas, CodeRerader const& a_code);

    void delay();
    void beep();
    void drawSprite(U8Bit a_x, U8Bit a_y, U16Bit a_spriteAddress, U8Bit a_lines);
    void exec();

private:
    void loadCharacters();
    void loadCode(CodeRerader const& a_code);
    U16Bit readInstruction(U16Bit a_address);
    auto instructionSetInit();

private:
    Memory m_memory;
    CallStack m_stack;
    Registers m_registers;
    InstructionSet m_instructionSet;
    KeyBoard& m_keyBoard;
    Canvas& m_canvas;
    U16Bit const m_DELAY_TIME = 100; //msec
    U16Bit const m_SOUND_FREQUENCY = 15'000; //hz
    
};

}   //namespace chip8

#endif //VM_HPP
