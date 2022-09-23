#pragma once

#include "settings.hpp"
#include "code_reader.hpp"

namespace chip8
{
    struct MemoryInitializer
    {
        static Memory make();
        static void loadCode(Memory &a_memory, CodeReader const &a_codeReader);
    };

} // namespace chip8