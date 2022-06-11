#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include<string>
#include<unordered_map>
#include"array.hpp"
#include"stack.hpp"

namespace chip8
{

u_short const MEMORY_CAPACITY = 4096;
using Memory = Array<u_char, MEMORY_CAPACITY>;

u_short const REGISTERS_CAPACITY = 16;
using Registers = Array<u_short, REGISTERS_CAPACITY>;

u_short const KEYS_CAPACITY = 16;
using Keys = Array<u_char, KEYS_CAPACITY>;
using KeysMap = std::unordered_map<char, u_char>;

u_short const STACK_CAPACITY = 16;
using CallStack = Stack<u_short, STACK_CAPACITY>;

u_short const CANVAS_WIDTH = 64;
u_short const CANVAS_HEIGHT = 32;
u_short const CANVAS_SCALE = 10;

std::string const TITLE = "Chip8 Emulator";

}   //namespace ship8

#endif // SETTINGS_HPP