#ifndef MANEGER_HPP
#define MANEGER_HPP

#include "settings.hpp"
#include "io.hpp"
#include "canvas.hpp"
#include "vm.hpp"

namespace chip8
{

class Maneger
{
public:
    Maneger()
    :   m_screen{CANVAS_WIDTH, CANVAS_HEIGHT, CANVAS_SCALE}
    ,   m_renderer{m_screen, CANVAS_SCALE, BLACK, WHITE}
    ,   m_canvas{m_renderer, CANVAS_WIDTH, CANVAS_HEIGHT}
    ,   m_keyBoard{KEY_BOARD}
    ,   m_loop{[&](U8Bit a_key){m_keyBoard.keyDown(a_key);}, [&](U8Bit a_key){m_keyBoard.keyUp(a_key);}}
    ,   m_vm{m_keyBoard, m_canvas}
    {
;
    }

    void mainLoop()
    {
        while(true)
        {
            m_loop();
            m_vm.delay();
            m_vm.beep();
        }
    }

private:
    Screen m_screen;
    Renderer m_renderer;
    Canvas m_canvas;
    KeyBoard m_keyBoard;
    EventLoop m_loop;
    VirtualMachine m_vm;

};

}   //namespace chip8

#endif //MANEGER_HPP