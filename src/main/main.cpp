#include <iostream>
#include "code_reader.hpp"
#include "main_loop_runner.hpp"

int main(int argc, char **argv)
{

    if (argc < 2)
    {
        std::cout << "You must provide a file to load\n";
        return -1;
    }

    using namespace chip8;
    std::filesystem::path filePath{argv[1]};
    CodeRerader code{filePath};
    auto config = StartupConfiguration{};
    MainLoopRunner manager{code, config};
    manager.runMainLoop();

    return 0;
}
