#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <cassert>

namespace chip8
{

template<typename T, size_t CAPACITY>
class Array
{
public:
    T& operator[](u_short a_index)
    {
        assert(a_index < CAPACITY);
        return m_memory[a_index];
    }

    T operator[](u_short a_index) const
    {
        assert(a_index < CAPACITY);
        return m_memory[a_index];
    }

private:
    T m_memory[CAPACITY];
};

}   //namespace chip8

#endif //ARRAY_HPP