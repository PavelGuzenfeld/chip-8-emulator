#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <memory>
#include "settings.hpp"
#include "renderer.hpp"

namespace chip8
{

    class Canvas
    {
    public:
        Canvas(std::shared_ptr<Renderer> a_renderer, U8Bit a_width, U8Bit a_height);

        bool setPixel(U8Bit a_x, U8Bit a_y);
        bool drawBits(U8Bit a_x, U8Bit a_y, U8Bit a_bits);
        bool isPixelOn(U8Bit a_x, U8Bit a_y);

        void clear();

    private:
        U16Bit index(U8Bit a_x, U8Bit a_y);

    private:
        Pixels m_pixels;
        std::shared_ptr<Renderer> m_renderer;
        U16Bit const m_WIDTH;
        U16Bit const m_HEIGHT;
    };

} // namespace chip8

#endif // CANVAS_HPP