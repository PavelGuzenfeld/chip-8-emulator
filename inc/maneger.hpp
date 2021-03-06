#ifndef MANEGER_HPP
#define MANEGER_HPP

#include "settings.hpp"
#include "io.hpp"
#include "canvas.hpp"
#include "vm.hpp"
#include "code_reader.hpp"

namespace chip8
{

class Maneger
{
public:
    Maneger(CodeRerader const& a_reader);

    void mainLoop();

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