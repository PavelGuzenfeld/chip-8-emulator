#pragma once

#include <filesystem>
#include "settings.hpp"

namespace chip8
{
    struct CodeReader
    {
        explicit CodeReader(std::filesystem::path const &a_file);

        size_t size;
        char buffer[MEMORY_CAPACITY];
    };

} // namespace chip8
