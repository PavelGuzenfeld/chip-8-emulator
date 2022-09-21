#pragma once

#include "settings.hpp"
#include "abstract.hpp"
namespace chip8
{

    class Renderer : public abc::Abstract
    {
    public:
        Renderer() = default;

        virtual void setPixel(U8Bit a_x, U8Bit a_y) = 0;
        virtual void resetPixel(U8Bit a_x, U8Bit a_y) = 0;

        virtual void present() = 0;
        virtual void clear() = 0;
    };
} // namespace chip8