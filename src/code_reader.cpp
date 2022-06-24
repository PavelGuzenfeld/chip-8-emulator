#include <cassert>  // assert()
#include <fstream>  // ifsream()
#include <iostream> // cout()
#include "settings.hpp"
#include "code_reader.hpp"

namespace chip8
{

CodeRerader::CodeRerader(std::filesystem::path const& a_file)
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

}   //namespace chip8