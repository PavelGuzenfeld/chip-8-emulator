#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include "cassert"
#include "settings.hpp"

namespace chip8
{

class KeyBoard
{
public:
    KeyBoard(KeysMap const& a_map)
    :   m_keyMap{a_map}
    ,   m_keysState{}
    {

    }
    
    void keyDown(U8Bit a_key)
    {
        auto index = m_keyMap.at(a_key);
        m_keysState[index] = true;
    }

    void keyUp(U8Bit a_key)
    {
        auto index = m_keyMap.at(a_key);
        m_keysState[index] = false;
    }

    bool isVirtualKeyDown(U8Bit a_key)
    {
        assert(a_key < KEYS_CAPACITY);
        return m_keysState[a_key];
    }

private:
    KeysMap m_keyMap;
    Keys m_keysState;

};

}   //namespace chip8

#endif //KEYBOARD_HPP