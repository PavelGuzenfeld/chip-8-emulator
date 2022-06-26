#include "maneger.hpp"

namespace chip8
{

Maneger::Maneger(CodeRerader const& a_reader)
    :   m_screen{CANVAS_WIDTH, CANVAS_HEIGHT, CANVAS_SCALE}
    ,   m_renderer{m_screen, CANVAS_SCALE, BLACK, WHITE}
    ,   m_canvas{m_renderer, CANVAS_WIDTH, CANVAS_HEIGHT}
    ,   m_keyBoard{KEY_BOARD}
    ,   m_loop{[&](U8Bit a_key){m_keyBoard.keyDown(a_key);}, [&](U8Bit a_key){m_keyBoard.keyUp(a_key);}}
    ,   m_vm{m_keyBoard, m_canvas, a_reader}
    {
        
    }

void Maneger::mainLoop()
{
    while(true)
    {
        m_loop();
        m_vm.delay();
        m_vm.beep();
    }
}

}   //namespace chip8