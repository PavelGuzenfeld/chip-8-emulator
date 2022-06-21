#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "settings.hpp"
#include "io.hpp"

namespace chip8
{

class Canvas
{
public:
    Canvas(Renderer& a_renderer, U8Bit a_width, U8Bit a_height)
    :   m_pixels{}   
    ,   m_renderer(a_renderer)
    ,   m_WIDTH(a_width)
    ,   m_HEIGHT(a_height)
    {

    }

    bool setPixel(U8Bit a_x, U8Bit a_y)
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

    bool isPixelOn(U8Bit a_x, U8Bit a_y)
    {
        U16Bit i = index(a_x, a_y);
        return m_pixels[i];
    }

    void clear()
    {
        m_renderer.clear();
    }

private:
    U16Bit index(U8Bit a_x, U8Bit a_y)
    {
        U8Bit x = a_x % m_WIDTH;
        U8Bit y = a_y % m_HEIGHT;
        return x + y * m_WIDTH;
    }

private:
    Pixels m_pixels;
    Renderer& m_renderer;
    U16Bit const m_WIDTH;
    U16Bit const m_HEIGHT;
    
};

}   //namespace chip8

#endif // CANVAS_HPP