#include <iostream>
#include "io.hpp"
#include "memory_initializer.hpp"
#include "code_reader.hpp"
#include "main_loop_runner.hpp"

void run_chip8_SDL_VM(chip8::CodeReader const &a_code)
{
    using namespace chip8;
    auto config = StartupConfiguration{};
    auto renderer = std::make_shared<RendererSDL>(config);
    auto canvas = Canvas{renderer, CANVAS_WIDTH, CANVAS_HEIGHT};
    auto keyBoard = KeyBoard(KEY_BOARD);
    auto memory = MemoryInitializer::make();
    MemoryInitializer::loadCode(memory, a_code);
    auto callStack = CallStack{};
    auto registers = Registers{};
    auto bus = Bus{memory, callStack, registers, keyBoard, canvas};
    auto instructionset = Instructionset{bus};
    auto loop = std::make_shared<EventLoopSDL>([&](U8Bit a_key)
                                               { keyBoard.keyDown(a_key); },
                                               [&](U8Bit a_key)
                                               { keyBoard.keyUp(a_key); });
    MainLoopRunner{bus, instructionset, loop}.run();
}

int main(int argc, char **argv)
{
    using namespace chip8;
    if (argc < 2)
    {
        std::cout << "You must provide a file to load\n";
        return -1;
    }

    auto path = std::filesystem::path{argv[1]};
    auto code = CodeReader{path};
    run_chip8_SDL_VM(code);
    return 0;
}
