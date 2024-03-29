#pragma once

#include <string>
#include <unordered_map>
#include <map>
#include <functional>
#include <array>
#include "lib/stack.hpp"

namespace chip8
{

    using U8Bit = unsigned char;
    using U16Bit = unsigned short;

    U16Bit const MEMORY_CAPACITY = 4096;
    using Memory = std::array<U8Bit, MEMORY_CAPACITY>;

    U16Bit const VX_CAPACITY = 16;
    using Vx = std::array<U8Bit, VX_CAPACITY>;

    U16Bit const KEYS_CAPACITY = 16;
    using Keys = std::array<U8Bit, KEYS_CAPACITY>;
    using KeysMap = std::unordered_map<U8Bit, U8Bit>;
    using OnKey = std::function<void(U8Bit)>;
    using OnKeyDown = std::function<U8Bit()>;

    U16Bit const STACK_CAPACITY = 16;
    using CallStack = Stack<U16Bit, STACK_CAPACITY>;

    U16Bit const CHAR_SET_CAPACITY = 16 * 5;
    using CharSet = std::array<U8Bit, CHAR_SET_CAPACITY>;

    U8Bit const CANVAS_WIDTH = 64;
    U8Bit const CANVAS_HEIGHT = 32;
    U8Bit const CANVAS_SCALE = 10;
    using Pixels = std::array<U8Bit, CANVAS_WIDTH * CANVAS_HEIGHT>;

    std::string const TITLE = "Chip8 Emulator";

    struct Color
    {
        U8Bit r;
        U8Bit g;
        U8Bit b;
        U8Bit a;
    };
    Color const BLACK = {};
    Color const WHITE = {255, 255, 255, 0};

    U16Bit const CODE_LOAD_ADDRESS = 0x200;

    using InstructionSet = std::map<U16Bit, std::function<void(U16Bit)>>;

    U16Bit const REFRESH_RATE_MS = 1;
    U16Bit const SOUND_FREQUENCY = 15'000;

    struct StartupConfiguration
    {
        U8Bit const WIDTH = CANVAS_WIDTH;
        U8Bit const HEIGHT = CANVAS_HEIGHT;
        U8Bit const SCALE = CANVAS_SCALE;
        Color const FORE = WHITE;
        Color const BACK = BLACK;
    };
} // namespace chip8
