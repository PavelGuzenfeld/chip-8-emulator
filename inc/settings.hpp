#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <string>
#include <unordered_map>
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

}   //namespace ship8

#endif // SETTINGS_HPP