#include <stdexcept>
#include "keyboard.hpp"
#include "io.hpp"

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

bool KeyBoard::isVirtualKeyDown(U8Bit a_key) const
{
    assert(a_key < KEYS_CAPACITY);
    return m_keysState[a_key];
}

U8Bit KeyBoard::waitForKeyPress()
{
    auto keyDownMethod = [&](U8Bit a_key){keyDown(a_key);};
    auto detailFn = waitForKeyPressFactory(keyDownMethod);
    return detailFn();
}

OnKey onKeyUp(KeyBoard& a_keyBoard)
{
    return [&](U8Bit a_key){a_keyBoard.keyUp(a_key);};
}

OnKey onKeyDown(KeyBoard& a_keyBoard)
{
    return [&](U8Bit a_key){a_keyBoard.keyDown(a_key);};
}




}   // namespace cpp