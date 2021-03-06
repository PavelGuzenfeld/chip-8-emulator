#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "settings.hpp"
#include "io.hpp"

namespace chip8
{

class Canvas
{
public:
    Canvas(Renderer& a_renderer, U8Bit a_width, U8Bit a_height);

    bool setPixel(U8Bit a_x, U8Bit a_y);
    bool drawBits(U8Bit a_x, U8Bit a_y, U8Bit a_bits);
    bool isPixelOn(U8Bit a_x, U8Bit a_y);

    void clear();

    void beep(U16Bit a_freq, U16Bit a_duration);

private:
    U16Bit index(U8Bit a_x, U8Bit a_y);

private:
    Pixels m_pixels;
    Renderer& m_renderer;
    U16Bit const m_WIDTH;
    U16Bit const m_HEIGHT;
    
};

}   //namespace chip8

#endif // CANVAS_HPP