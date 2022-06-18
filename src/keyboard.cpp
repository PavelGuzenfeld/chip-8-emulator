#include <stdexcept>
#include "keyboard.hpp"

namespace chip8
{

KeyBoard::KeyBoard(KeysMap const& a_map)
:   m_keyMap{a_map}
,   m_keysState{}
{

}

void KeyBoard::keyDown(U8Bit a_key)
try
{
    auto index = m_keyMap.at(a_key);
    m_keysState[index] = true;
}
catch(std::out_of_range& e)
{
    return;
}

void KeyBoard::keyUp(U8Bit a_key)
try
{
    auto index = m_keyMap.at(a_key);
    m_keysState[index] = false;
}
catch(std::out_of_range& e)
{
    return;
}

bool KeyBoard::isVirtualKeyDown(U8Bit a_key)
{
    assert(a_key < KEYS_CAPACITY);
    return m_keysState[a_key];
}

}   // namespace cpp