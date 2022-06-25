#include "canvas.hpp"

namespace chip8
{

Canvas::Canvas(Renderer& a_renderer, U8Bit a_width, U8Bit a_height)
    :   m_pixels{}   
    ,   m_renderer(a_renderer)
    ,   m_WIDTH(a_width)
    ,   m_HEIGHT(a_height)
    {

    }

bool Canvas::setPixel(U8Bit a_x, U8Bit a_y)
{
    auto i = index(a_x, a_y);
    auto wasTrue =  m_pixels[i];
    m_pixels[i] ^= true;
    if(wasTrue)
    {
        m_renderer.resetPixel(a_x, a_y);
    }
    else
    {
        m_renderer.setPixel(a_x, a_y);
    }
    m_renderer.present();
    return wasTrue;
}

bool Canvas::isPixelOn(U8Bit a_x, U8Bit a_y)
{
    U16Bit i = index(a_x, a_y);
    return m_pixels[i];
}

void Canvas::clear()
{
    m_renderer.clear();
}

void Canvas::beep(U16Bit a_freq, U16Bit a_duration)
{
    m_renderer.beep(a_freq, a_duration);
}

U16Bit Canvas::index(U8Bit a_x, U8Bit a_y)
{
    U8Bit x = a_x % m_WIDTH;
    U8Bit y = a_y % m_HEIGHT;
    return x + y * m_WIDTH;
}

}   //namespace chip8
