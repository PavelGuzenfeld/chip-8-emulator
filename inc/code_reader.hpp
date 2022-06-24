#ifndef CODE_READER_HPP
#define CODE_READER_HPP

#include <cassert>
#include <fstream>
#include <filesystem>
#include "settings.hpp"

namespace chip8
{
    struct CodeRerader
    {
        CodeRerader(std::filesystem::path const& a_file)
        try
        {
            auto static const MAX_SIZE = MEMORY_CAPACITY - CODE_LOAD_ADDRESS;
            auto const SIZE = std::filesystem::file_size(a_file);
            assert(SIZE < MAX_SIZE);
            std::ifstream fin(a_file, std::ios::in | std::ios::binary);
            fin.read(buffer, MEMORY_CAPACITY);
        }
        catch(std::filesystem::filesystem_error& e)
        {
            std::cout << e.what() << '\n';
        }
        
        char buffer[MEMORY_CAPACITY];
    };

}   //namespace chip8

#endif //CODE_READER_HPP

//TODO: add exceptions failed to open file .