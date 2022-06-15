#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include<string>
#include<unordered_map>
#include"lib/array.hpp"
#include"lib/stack.hpp"

namespace chip8
{

using U8Bit = unsigned char;
using U16Bit = unsigned short;

U16Bit const MEMORY_CAPACITY = 4096;
using Memory = Array<U8Bit, MEMORY_CAPACITY>;

U16Bit const REGISTERS_CAPACITY = 16;
using Registers = Array<U8Bit, REGISTERS_CAPACITY>;

U16Bit const KEYS_CAPACITY = 16;
using Keys = Array<U8Bit, KEYS_CAPACITY>;
using KeysMap = std::unordered_map<U8Bit, U8Bit>;

U16Bit const STACK_CAPACITY = 16;
using CallStack = Stack<U16Bit, STACK_CAPACITY>;

U16Bit const CHAR_SET_CAPACITY = 16 * 5;
using CharSet = Array<U8Bit, CHAR_SET_CAPACITY>;
// auto const charSet = CharSet{
//     0xF0, 0x90, 0x90, 0x90, 0xF0, //0
//     0xF0, 0x90, 0x90, 0x90, 0xF0, //1
//     0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
//     0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
//     0x90, 0x90, 0xF0, 0x10, 0x10, //4
//     0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
//     0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
//     0xF0, 0x80, 0xF0, 0x90, 0xF0, //7
//     0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
//     0xF0, 0x90, 0xF0, 0x90, 0xF0, //9
//     0xF0, 0x90, 0xF0, 0x90, 0x90, //A
//     0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
//     0xF0, 0x80, 0x80, 0x80, 0xF0, //C
//     0xE0, 0x90, 0x90, 0x90, 0xE0, //D
//     0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
//     0xF0, 0x80, 0xF0, 0x80, 0x80, //F
// };

U16Bit const CANVAS_WIDTH = 64;
U16Bit const CANVAS_HEIGHT = 32;
U16Bit const CANVAS_SCALE = 10;
using Pixels = Array<U8Bit, CANVAS_WIDTH * CANVAS_WIDTH>;

std::string const TITLE = "Chip8 Emulator";

}   //namespace ship8

#endif // SETTINGS_HPP