#ifndef STACK_HPP
#define STACK_HPP

#include<stack>
#include<cassert>

namespace chip8
{

template<typename VAL_T, size_t CAPACITY>
class Stack
{
public:   
    void push(VAL_T a_data)
    {
        assert(m_stack.size < CAPACITY);
        m_stack.push(a_data);
    }

    VAL_T pop()
    {
        assert(m_stack.size > 0);
        return m_stack.pop();
    }
private:
    std::stack<VAL_T> m_stack;

};

}   //namespace chip8

#endif //STACk_HPP