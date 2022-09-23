#include <cassert>  // assert()
#include <fstream>  // ifsream()
#include <iostream> // cout()
#include "settings.hpp"
#include "code_reader.hpp"

namespace chip8
{

    CodeReader::CodeReader(std::filesystem::path const &a_file)
    try : size(std::filesystem::file_size(a_file)), buffer{}
    {
#ifndef NDEBUG
        auto static const MAX_SIZE = MEMORY_CAPACITY - CODE_LOAD_ADDRESS;
        // size = std::filesystem::file_size(a_file);
        assert(size < MAX_SIZE);
#endif
        std::ifstream fin(a_file, std::ios::binary);
        fin.read(buffer, size);
    }
    catch (std::filesystem::filesystem_error &e)
    {
        std::cout << e.what() << '\n';
    }

} // namespace chip8