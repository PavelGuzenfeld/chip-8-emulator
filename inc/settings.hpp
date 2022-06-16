#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <string>
#include <unordered_map>
#include "lib/array.hpp"
#include "lib/stack.hpp"

namespace chip8
{

using U8Bit = unsigned char;
using U16Bit = unsigned short;

U16Bit const MEMORY_CAPACITY = 4096;
using Memory = Array<U8Bit, MEMORY_CAPACITY>;

U16Bit const VX_CAPACITY = 16;
using Vx = Array<U8Bit, VX_CAPACITY>;

U16Bit const KEYS_CAPACITY = 16;
using Keys = Array<U8Bit, KEYS_CAPACITY>;
using KeysMap = std::unordered_map<U8Bit, U8Bit>;

U16Bit const STACK_CAPACITY = 16;
using CallStack = Stack<U16Bit, STACK_CAPACITY>;

U16Bit const CHAR_SET_CAPACITY = 16 * 5;
using CharSet = Array<U8Bit, CHAR_SET_CAPACITY>;

U16Bit const CANVAS_WIDTH = 64;
U16Bit const CANVAS_HEIGHT = 32;
U16Bit const CANVAS_SCALE = 10;
using Pixels = Array<U8Bit, CANVAS_WIDTH * CANVAS_WIDTH>;

std::string const TITLE = "Chip8 Emulator";

}   //namespace ship8

#endif // SETTINGS_HPP