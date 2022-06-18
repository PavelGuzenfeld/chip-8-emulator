#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include "cassert"
#include "settings.hpp"

namespace chip8
{

class KeyBoard
{
    
public:
    KeyBoard(KeysMap const& a_map);

    void keyDown(U8Bit a_key);
    void keyUp(U8Bit a_key);

    bool isVirtualKeyDown(U8Bit a_key);

private:
    KeysMap m_keyMap;
    Keys m_keysState;

};

}   //namespace chip8

#endif //KEYBOARD_HPP