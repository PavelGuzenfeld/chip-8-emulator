#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include "settings.hpp"

namespace chip8
{

class KeyBoard
{
    
public:
    KeyBoard(KeysMap const& a_map);

    void keyDown(U8Bit a_key);
    void keyUp(U8Bit a_key);

    bool isVirtualKeyDown(U8Bit a_key) const;

    U8Bit waitForKeyPress();

private:
    KeysMap m_keyMap;
    Keys m_keysState;
};

OnKey onKeyUp(KeyBoard& a_keyBoard);
OnKey onKeyDown(KeyBoard& a_keyBoard);




}   //namespace chip8

#endif //KEYBOARD_HPP