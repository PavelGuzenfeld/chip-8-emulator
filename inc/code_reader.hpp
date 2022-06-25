#ifndef CODE_READER_HPP
#define CODE_READER_HPP

#include <filesystem>
#include "settings.hpp"

namespace chip8
{
    struct CodeRerader
    {
        explicit CodeRerader(std::filesystem::path const& a_file);
        
        size_t size;
        char buffer[MEMORY_CAPACITY];
    };

}   //namespace chip8

#endif //CODE_READER_HPP
