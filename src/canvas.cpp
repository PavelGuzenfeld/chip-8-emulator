#include "canvas.hpp"
namespace chip8
{

    Canvas::Canvas(std::shared_ptr<Renderer> a_renderer, U8Bit a_width, U8Bit a_height)
        : m_pixels{}, m_renderer(a_renderer), m_WIDTH(a_width), m_HEIGHT(a_height)
    {
    }

    bool Canvas::setPixel(U8Bit a_x, U8Bit a_y)
    {
        auto i = index(a_x, a_y);
        auto wasTrue = m_pixels[i];
        m_pixels[i] ^= true;
        U8Bit x = a_x % m_WIDTH;
        U8Bit y = a_y % m_HEIGHT;
        if (wasTrue)
        {
            m_renderer->resetPixel(x, y);
        }
        else
        {
            m_renderer->setPixel(x, y);
        }
        m_renderer->present();
        return wasTrue;
    }

    bool Canvas::drawBits(U8Bit a_x, U8Bit a_y, U8Bit a_bits)
    {
        bool wasSet = false;
        U8Bit checkBit = 0b1000'0000;
        for (U8Bit offsetX = 0; offsetX < 8; ++offsetX)
        {
            if (a_bits & checkBit)
            {
                bool wasSet_inner = setPixel(a_x + offsetX, a_y);
                if (wasSet_inner)
                {
                    wasSet = wasSet_inner;
                }
            }
            checkBit >>= 1;
        }
        return wasSet;
    }

    bool Canvas::isPixelOn(U8Bit a_x, U8Bit a_y)
    {
        U16Bit i = index(a_x, a_y);
        return m_pixels[i];
    }

    void Canvas::clear()
    {
        for (U16Bit i = 0; i < m_WIDTH * m_HEIGHT; ++i)
        {
            m_pixels[i] = false;
        }
        m_renderer->clear();
    }

    U16Bit Canvas::index(U8Bit a_x, U8Bit a_y)
    {
        U8Bit x = a_x % m_WIDTH;
        U8Bit y = a_y % m_HEIGHT;
        return x + y * m_WIDTH;
    }

} // namespace chip8
