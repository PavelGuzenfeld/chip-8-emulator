#ifndef VM_HPP
#define VM_HPP

#include <string>
#include <array>

namespace chip8
{

std::string m_title = "Chip8 Emulator";
size_t const m_memorySize = 4096;
size_t const m_width = 64;
size_t const m_hight = 32;
size_t const m_windowScale = 10;

struct Memory
{
    void set(size_t a_index, unsigned char a_value);
    std::array<char, m_memorySize> m_memory;
};


}   //namespace chip8

#endif //VM_HPP