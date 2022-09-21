#include "mu_test.h"
#include <thread>
#include <SDL2/SDL.h>
#include <unistd.h> // sleep()

#define private public
#include "settings.hpp"
#include "registers.hpp"
#include "keyboard.hpp"
#include "io.hpp"
#include "vm.hpp"
#include "canvas.hpp"
#include "main_loop_runner.hpp"
#include "code_reader.hpp"

BEGIN_TEST(memory)
{
    using namespace chip8;
    auto m = Memory();
    for (u_short i = 0; i < MEMORY_CAPACITY; ++i)
    {
        ASSERT_EQUAL(m[i], 0);
    }

    for (U16Bit i = 0; i < MEMORY_CAPACITY; ++i)
    {
        m[i] = i % 256;
        // TRACER << "m[i] = " << std::to_string(m[i]) << "\n";
    }

    for (u_short i = 0; i < MEMORY_CAPACITY; ++i)
    {
        ASSERT_EQUAL(m[i], i % 256);
    }
}
END_TEST

BEGIN_TEST(registers)
{
    using namespace chip8;
    auto regs = Registers{};
    for (U16Bit i = 0; i < VX_CAPACITY; ++i)
    {
        ASSERT_EQUAL(regs.m_vx[i], 0);
    }
    ASSERT_EQUAL(regs.m_index, 0);
    ASSERT_EQUAL(regs.m_PC, 0);
    ASSERT_EQUAL(regs.m_delayTimer, 0);
    ASSERT_EQUAL(regs.m_soundTimer, 0);

    for (U16Bit i = 0; i < VX_CAPACITY; ++i)
    {
        regs.m_vx[i] = i;
    }
    for (U16Bit i = 0; i < VX_CAPACITY; ++i)
    {
        ASSERT_EQUAL(regs.m_vx[i], i);
    }

    regs.m_index = 42;
    ASSERT_EQUAL(regs.m_index, 42);

    regs.m_PC = 77;
    ASSERT_EQUAL(regs.m_PC, 77);

    regs.m_delayTimer = 87;
    ASSERT_EQUAL(regs.m_delayTimer, 87);

    regs.m_soundTimer = 97;
    ASSERT_EQUAL(regs.m_soundTimer, 97);
}
END_TEST

BEGIN_TEST(stack)
{
    using namespace chip8;
    auto stack = CallStack{};
    for (U16Bit i = 0; i < STACK_CAPACITY; ++i)
    {
        stack.push(i);
    }
    for (U16Bit i = 0; i < VX_CAPACITY; ++i)
    {
        ASSERT_EQUAL(stack.pop(), STACK_CAPACITY - i - 1);
    }
}
END_TEST

BEGIN_TEST(keyboard)
{
    using namespace chip8;
    auto keyBoard = KeyBoard(KEY_BOARD);
    for (U16Bit key = 0; key < KEYS_CAPACITY; ++key)
    {
        ASSERT_EQUAL(keyBoard.isVirtualKeyDown(key), false);
    }

    for (auto pairKey : KEY_BOARD)
    {
        auto key = pairKey.first;
        keyBoard.keyDown(key);
    }
    for (U16Bit key = 0; key < KEYS_CAPACITY; ++key)
    {
        ASSERT_EQUAL(keyBoard.isVirtualKeyDown(key), true);
    }

    for (auto pairKey : KEY_BOARD)
    {
        auto key = pairKey.first;
        keyBoard.keyUp(key);
    }
    for (U16Bit key = 0; key < KEYS_CAPACITY; ++key)
    {
        ASSERT_EQUAL(keyBoard.isVirtualKeyDown(key), false);
    }
}
END_TEST

BEGIN_TEST(keyboard_out_of_range)
{
    using namespace chip8;
    auto keyBoard = KeyBoard(KEY_BOARD);

    keyBoard.keyDown(0);
    ASSERT_PASS();

    keyBoard.keyUp(200);
    ASSERT_PASS();
}
END_TEST

BEGIN_TEST(screen)
{
    using namespace chip8;
    auto config = StartupConfiguration{};
    auto renderer = RendererSDL{config};

    renderer.clear();
    for (U8Bit x = 0; x < CANVAS_WIDTH; ++x)
    {
        for (U8Bit y = 0; y < CANVAS_HEIGHT; ++y)
        {
            renderer.setPixel(x, y);
        }
    }
    renderer.clear();
    ASSERT_PASS();
}
END_TEST

BEGIN_TEST(canvas)
{
    using namespace chip8;
    auto config = StartupConfiguration{};
    auto renderer = std::make_shared<RendererSDL>(config);
    auto canvas = Canvas{renderer, CANVAS_WIDTH, CANVAS_HEIGHT};

    for (U8Bit y = 0; y < CANVAS_HEIGHT; ++y)
    {
        for (U8Bit x = 0; x < CANVAS_WIDTH; ++x)
        {
            ASSERT_EQUAL(canvas.isPixelOn(x, y), false);
            auto wasTrue = canvas.setPixel(x, y);
            ASSERT_EQUAL(canvas.isPixelOn(x, y), true);
            ASSERT_EQUAL(wasTrue, false);
        }
    }

    for (U8Bit y = 0; y < CANVAS_HEIGHT; ++y)
    {
        for (U8Bit x = 0; x < CANVAS_WIDTH; ++x)
        {
            ASSERT_EQUAL(canvas.isPixelOn(x, y), true);
            auto wasTrue = canvas.setPixel(x, y);
            ASSERT_EQUAL(canvas.isPixelOn(x, y), false);
            ASSERT_EQUAL(wasTrue, true);
        }
    }
}
END_TEST

BEGIN_TEST(event_loop)
{
    using namespace chip8;

    auto keyBoard = KeyBoard{KEY_BOARD};
    auto loop = EventLoop{[&](U8Bit a_key)
                          { keyBoard.keyDown(a_key); },
                          [&](U8Bit a_key)
                          { keyBoard.keyUp(a_key); },
                          true};
    for (U8Bit key = 0; key < 16; ++key)
    {
        ASSERT_EQUAL(keyBoard.isVirtualKeyDown(key), false);
    }

    {
        auto job = [&]()
        { loop.run(); };
        auto worker = std::thread(job);
        for (auto key : KEY_BOARD)
        {
            simulateKeyEvent(SDL_KEYDOWN, static_cast<SDL_EventType>(key.first));
        }
        simulateExit();
        worker.join();

        for (U8Bit key = 0; key < 16; ++key)
        {
            ASSERT_EQUAL(keyBoard.isVirtualKeyDown(key), true);
        }
    }

    {
        auto job = [&]()
        { loop.run(); };
        auto worker = std::thread(job);
        for (auto key : KEY_BOARD)
        {
            simulateKeyEvent(SDL_KEYUP, static_cast<SDL_EventType>(key.first));
        }
        simulateExit();
        worker.join();

        for (U8Bit key = 0; key < 16; ++key)
        {
            ASSERT_EQUAL(keyBoard.isVirtualKeyDown(key), false);
        }
    }
}
END_TEST

BEGIN_TEST(code_reader)
{
    using namespace chip8;
    auto code = CodeRerader{"./test/code_reader_test.file"};
    // TRACER << code.buffer << "\n";
    ASSERT_EQUAL_STR(code.buffer, "hello world!");
}
END_TEST

BEGIN_TEST(print_sprites)
{
    using namespace chip8;
    auto config = StartupConfiguration{};
    auto renderer = std::make_shared<RendererSDL>(config);
    auto canvas = Canvas{renderer, CANVAS_WIDTH, CANVAS_HEIGHT};
    auto keyBoard = KeyBoard(KEY_BOARD);
    auto code = CodeRerader{"./test/code_reader_test.file"};
    auto vm = VirtualMachine{keyBoard, canvas, code};
    vm.delay();
    vm.beep();
    auto x = 0;
    for (U8Bit i = 0; i < 0x10; i += 4)
    {
        vm.m_instructionset.drawSprite(x, 0, i, 5);
        x += 5;
        sleep(1);
    }

    for (U8Bit i = 0; i < 0x10; ++i)
    {
        vm.m_instructionset.drawSprite(61, 31, i, 5);
    }
    ASSERT_PASS();
}
END_TEST

BEGIN_TEST(display_opcodes)
{
    using namespace chip8;
    auto config = StartupConfiguration{};
    auto renderer = std::make_shared<RendererSDL>(config);
    auto canvas = Canvas{renderer, CANVAS_WIDTH, CANVAS_HEIGHT};
    auto keyBoard = KeyBoard(KEY_BOARD);
    auto code = CodeRerader{"./test/test.bin"};
    auto vm = VirtualMachine{keyBoard, canvas, code};

    // 0xdfff opcode
    auto const OPCODE = 0xdab5;
    {
        vm.m_registers.m_vx[0xa] = 0;
        vm.m_registers.m_vx[0xb] = 0;
        vm.m_registers.m_index = 0;
        for (auto i = 0x0; i < 0xa; ++i)
        {
            vm.m_instructionset.runInstraction(OPCODE);
            vm.m_registers.m_index += 5;
            vm.m_registers.m_vx[0xa] += 5;
            ASSERT_EQUAL(vm.m_registers.m_vx[0xf], 0);
        }
        vm.m_registers.m_vx[0xb] = 10;
        for (auto i = 0xa; i < 0x10; ++i)
        {
            vm.m_instructionset.runInstraction(OPCODE);
            vm.m_registers.m_index += 5;
            vm.m_registers.m_vx[0xa] += 5;
            ASSERT_EQUAL(vm.m_registers.m_vx[0xf], 0);
        }
    }
    {
        vm.m_registers.m_vx[0xa] = 0;
        vm.m_registers.m_vx[0xb] = 0;
        vm.m_registers.m_index = 0;
        for (auto i = 0x0; i < 0xa; ++i)
        {
            vm.m_instructionset.runInstraction(OPCODE);
            vm.m_registers.m_index += 5;
            vm.m_registers.m_vx[0xa] += 5;
            ASSERT_EQUAL(vm.m_registers.m_vx[0xf], 1);
        }
        vm.m_registers.m_vx[0xb] = 10;
        for (auto i = 0xa; i < 0x10; ++i)
        {
            vm.m_instructionset.runInstraction(OPCODE);
            vm.m_registers.m_index += 5;
            vm.m_registers.m_vx[0xa] += 5;
            ASSERT_EQUAL(vm.m_registers.m_vx[0xf], 1);
        }
    }
}
END_TEST

BEGIN_TEST(opCode_execution)
{
    using namespace chip8;
    auto config = StartupConfiguration{};
    auto renderer = std::make_shared<RendererSDL>(config);
    auto canvas = Canvas{renderer, CANVAS_WIDTH, CANVAS_HEIGHT};
    auto keyBoard = KeyBoard(KEY_BOARD);
    auto code = CodeRerader{"./test/test.bin"};
    auto vm = VirtualMachine{keyBoard, canvas, code};
    {
        // 0x00ee
        vm.m_stack.push(0xaa);
        vm.m_instructionset.runInstraction(0x00ee);
        ASSERT_EQUAL(vm.m_registers.m_PC, 0xaa);
    }

    {
        // ox1nnn
        vm.m_instructionset.runInstraction(0x1aaa);
        ASSERT_EQUAL(vm.m_registers.m_PC, 0xaaa);
    }

    {
        // 0x2nnn
        vm.m_registers.m_PC = 0xaaa;
        vm.m_instructionset.runInstraction(0x2bbb);
        ASSERT_EQUAL(vm.m_registers.m_PC, 0xbbb);
        ASSERT_EQUAL(vm.m_stack.pop(), 0xaaa);
    }

    {
        // 0X3xnn
        vm.m_registers.m_PC = 0;
        vm.m_registers.m_vx[0xa] = 0xbb;
        vm.m_instructionset.runInstraction(0x3abb);
        ASSERT_EQUAL(vm.m_registers.m_PC, 2);
    }

    {
        // 0x4xnn
        vm.m_registers.m_PC = 0;
        vm.m_registers.m_vx[0xa] = 0xbb;
        vm.m_instructionset.runInstraction(0x4abb);
        ASSERT_EQUAL(vm.m_registers.m_PC, 0);
    }

    {
        // 0x5xy0
        vm.m_registers.m_PC = 0;
        vm.m_registers.m_vx[0xa] = 0xbb;
        vm.m_registers.m_vx[0xb] = 0xbb;
        vm.m_instructionset.runInstraction(0x5ab0);
        ASSERT_EQUAL(vm.m_registers.m_PC, 2);
    }

    {
        // 0x6xnn
        vm.m_instructionset.runInstraction(0x6abb);
        ASSERT_EQUAL(vm.m_registers.m_vx[0xa], 0xbb);
    }

    {
        // 0x7xnn
        vm.m_registers.m_vx[0xa] = 0x1;
        vm.m_instructionset.runInstraction(0x7abb);
        ASSERT_EQUAL(vm.m_registers.m_vx[0xa], 0xbc);
    }

    {
        // 0x8xy0
        vm.m_registers.m_vx[0xa] = 0x1;
        vm.m_registers.m_vx[0xb] = 0x55;
        vm.m_instructionset.runInstraction(0x8ab0);
        ASSERT_EQUAL(vm.m_registers.m_vx[0xa], 0x55);
    }

    {
        // 0x8xy1
        vm.m_registers.m_vx[0xa] = 0x0;
        vm.m_registers.m_vx[0xb] = 0x1;
        vm.m_instructionset.runInstraction(0x8ab1);
        ASSERT_EQUAL(vm.m_registers.m_vx[0xa], 0x1);
    }

    {
        // 0x8xy2
        vm.m_registers.m_vx[0xa] = 0x0f;
        vm.m_registers.m_vx[0xb] = 0x03;
        vm.m_instructionset.runInstraction(0x8ab2);
        ASSERT_EQUAL(vm.m_registers.m_vx[0xa], 0x03);
    }

    {
        // 0x8xy3
        vm.m_registers.m_vx[0xa] = 0x0f;
        vm.m_registers.m_vx[0xb] = 0x03;
        vm.m_instructionset.runInstraction(0x8ab3);
        ASSERT_EQUAL(vm.m_registers.m_vx[0xa], 0x0c);
    }

    {
        // 0x8xy4
        vm.m_registers.m_vx[0xa] = 0xff;
        vm.m_registers.m_vx[0xb] = 0x03;
        vm.m_registers.m_vx[0xf] = 0;
        vm.m_instructionset.runInstraction(0x8ab4);
        ASSERT_EQUAL(vm.m_registers.m_vx[0xa], static_cast<U8Bit>(0xff + 0x03));
        ASSERT_EQUAL(vm.m_registers.m_vx[0xf], 1);
    }

    {
        // 0x8xy4
        vm.m_registers.m_vx[0xa] = 0x04;
        vm.m_registers.m_vx[0xb] = 0x03;
        vm.m_registers.m_vx[0xf] = 0;
        vm.m_instructionset.runInstraction(0x8ab4);
        ASSERT_EQUAL(vm.m_registers.m_vx[0xa], static_cast<U8Bit>(0x04 + 0x03));
        ASSERT_EQUAL(vm.m_registers.m_vx[0xf], 0);
    }

    {
        // 0x8xy5
        vm.m_registers.m_vx[0xa] = 0x04;
        vm.m_registers.m_vx[0xb] = 0x03;
        vm.m_registers.m_vx[0xf] = 0;
        vm.m_instructionset.runInstraction(0x8ab5);
        ASSERT_EQUAL(vm.m_registers.m_vx[0xa], static_cast<U8Bit>(0x04 - 0x03));
        ASSERT_EQUAL(vm.m_registers.m_vx[0xf], 0);
    }

    {
        // 0x8xy5
        vm.m_registers.m_vx[0xa] = 0x03;
        vm.m_registers.m_vx[0xb] = 0x04;
        vm.m_registers.m_vx[0xf] = 0;
        vm.m_instructionset.runInstraction(0x8ab5);
        ASSERT_EQUAL(vm.m_registers.m_vx[0xa], static_cast<U8Bit>(0x03 - 0x04));
        ASSERT_EQUAL(vm.m_registers.m_vx[0xf], 1);
    }

    {
        // 0x8xy6
        vm.m_registers.m_vx[0xa] = 0x03;
        vm.m_registers.m_vx[0xb] = 0x04;
        vm.m_registers.m_vx[0xf] = 0;
        vm.m_instructionset.runInstraction(0x8ab6);
        ASSERT_EQUAL(vm.m_registers.m_vx[0xa], static_cast<U8Bit>(0x03 >> 1));
        ASSERT_EQUAL(vm.m_registers.m_vx[0xf], 1);
    }

    {
        // 0x8xy6
        vm.m_registers.m_vx[0xa] = 0x02;
        vm.m_registers.m_vx[0xb] = 0x04;
        vm.m_registers.m_vx[0xf] = 0;
        vm.m_instructionset.runInstraction(0x8ab6);
        ASSERT_EQUAL(vm.m_registers.m_vx[0xa], static_cast<U8Bit>(0x02 >> 1));
        ASSERT_EQUAL(vm.m_registers.m_vx[0xf], 0);
    }

    {
        // 0x8xy7
        vm.m_registers.m_vx[0xa] = 0x03;
        vm.m_registers.m_vx[0xb] = 0x04;
        vm.m_registers.m_vx[0xf] = 0;
        vm.m_instructionset.runInstraction(0x8ab7);
        ASSERT_EQUAL(vm.m_registers.m_vx[0xa], static_cast<U8Bit>(0x04 - 0x03));
        ASSERT_EQUAL(vm.m_registers.m_vx[0xf], 1);
    }

    {
        // 0x8xy7
        vm.m_registers.m_vx[0xa] = 0x04;
        vm.m_registers.m_vx[0xb] = 0x03;
        vm.m_registers.m_vx[0xf] = 0;
        vm.m_instructionset.runInstraction(0x8ab7);
        ASSERT_EQUAL(vm.m_registers.m_vx[0xa], static_cast<U8Bit>(0x03 - 0x04));
        ASSERT_EQUAL(vm.m_registers.m_vx[0xf], 0);
    }

    {
        // 0x8xyE
        vm.m_registers.m_vx[0xa] = 0x0ff;
        vm.m_registers.m_vx[0xb] = 0x04;
        vm.m_registers.m_vx[0xf] = 0;
        vm.m_instructionset.runInstraction(0x8abe);
        ASSERT_EQUAL(vm.m_registers.m_vx[0xa], static_cast<U8Bit>(0x0ff << 1));
        ASSERT_EQUAL(vm.m_registers.m_vx[0xf], 1);
    }

    {
        // 0x8xyE
        vm.m_registers.m_vx[0xa] = 0x02;
        vm.m_registers.m_vx[0xb] = 0x04;
        vm.m_registers.m_vx[0xf] = 0;
        vm.m_instructionset.runInstraction(0x8abe);
        ASSERT_EQUAL(vm.m_registers.m_vx[0xa], static_cast<U8Bit>(0x02 << 1));
        ASSERT_EQUAL(vm.m_registers.m_vx[0xf], 0);
    }

    {
        // 0x9xy0
        vm.m_registers.m_vx[0xa] = 0x02;
        vm.m_registers.m_vx[0xb] = 0x04;
        vm.m_registers.m_vx[0xf] = 0;
        vm.m_instructionset.runInstraction(0x9ab0);
        ASSERT_EQUAL(vm.m_registers.m_vx[0xf], 0);
    }

    {
        // 0xafff
        vm.m_registers.m_index = 0;
        vm.m_instructionset.runInstraction(0xaaaa);
        ASSERT_EQUAL(vm.m_registers.m_index, 0xaaa);
    }

    {
        // 0xbfff
        vm.m_registers.m_vx[0] = 5;
        vm.m_instructionset.runInstraction(0xbaaa);
        ASSERT_EQUAL(vm.m_registers.m_PC, 5 + 0xaaa);
    }

    {
        // 0xcfff
        auto sum = 0;
        for (auto i = 0; i < 10000; ++i)
        {
            vm.m_instructionset.runInstraction(0xcfff);
            sum += vm.m_registers.m_vx[0xf];
        }
        ASSERT_EQUAL(static_cast<int>(sum / 10000), int(255 / 2));
    }

    {
        // 0xex9e
        vm.m_registers.m_vx[0] = 0x01;
        vm.m_registers.m_PC = 0x200;
        keyBoard.keyDown(SDLK_2);
        vm.m_instructionset.runInstraction(0xe09e);
        ASSERT_EQUAL(vm.m_registers.m_PC, 0x200);
    }

    {
        // 0xex9e
        vm.m_registers.m_vx[0] = 0x02;
        vm.m_registers.m_PC = 0x200;
        keyBoard.keyDown(SDLK_2);
        vm.m_instructionset.runInstraction(0xe09e);
        ASSERT_EQUAL(vm.m_registers.m_PC, 0x202);
    }

    {
        // 0xexa1
        vm.m_registers.m_vx[0] = 0x01;
        vm.m_registers.m_PC = 0x200;
        keyBoard.keyDown(SDLK_2);
        vm.m_instructionset.runInstraction(0xe0a1);
        ASSERT_EQUAL(vm.m_registers.m_PC, 0x202);
    }

    {
        // 0xexa1
        vm.m_registers.m_vx[0] = 0x02;
        vm.m_registers.m_PC = 0x200;
        keyBoard.keyDown(SDLK_2);
        vm.m_instructionset.runInstraction(0xe0a1);
        ASSERT_EQUAL(vm.m_registers.m_PC, 0x200);
    }

    {
        // 0xfx07
        vm.m_registers.m_vx[0] = 0x01;
        vm.m_registers.m_delayTimer = 0x02;
        vm.m_instructionset.runInstraction(0xf007);
        ASSERT_EQUAL(vm.m_registers.m_vx[0], 0x02);
    }

    {
        // 0xfx0a
        vm.m_registers.m_vx[0] = 0x01;
        auto job = [&]()
        { vm.m_instructionset.runInstraction(0xf00a); };
        auto worker = std::thread(job);
        simulateKeyEvent(SDL_KEYDOWN, SDLK_2);
        worker.join();
        ASSERT_EQUAL(vm.m_registers.m_vx[0], 0x02);
    }

    {
        // 0xfx15
        vm.m_registers.m_vx[0] = 0x01;
        vm.m_instructionset.runInstraction(0xf015);
        ASSERT_EQUAL(vm.m_registers.m_delayTimer, 0x01);
    }

    {
        // 0xfx18
        vm.m_registers.m_vx[0] = 0x01;
        vm.m_instructionset.runInstraction(0xf018);
        ASSERT_EQUAL(vm.m_registers.m_soundTimer, 0x01);
    }

    {
        // 0xfx1e
        vm.m_registers.m_vx[0] = 0x01;
        vm.m_registers.m_index = 0x02;
        vm.m_instructionset.runInstraction(0xf01e);
        ASSERT_EQUAL(vm.m_registers.m_index, 0x03);
    }

    {
        // 0xfx29
        vm.m_registers.m_vx[0] = 0x01;
        vm.m_instructionset.runInstraction(0xf029);
        ASSERT_EQUAL(vm.m_registers.m_index, 0x01 * 5);
    }

    {
        // 0xfx33
        vm.m_registers.m_vx[0] = 123;
        vm.m_instructionset.runInstraction(0xf033);
        ASSERT_EQUAL(vm.m_memory[vm.m_registers.m_index], 1);
        ASSERT_EQUAL(vm.m_memory[vm.m_registers.m_index + 1], 2);
        ASSERT_EQUAL(vm.m_memory[vm.m_registers.m_index + 2], 3);
    }

    {
        // 0xfx33
        // case vx = 0x0
        vm.m_registers.m_vx[0] = 0;
        vm.m_instructionset.runInstraction(0xf033);
        ASSERT_EQUAL(vm.m_memory[vm.m_registers.m_index], 0);
        ASSERT_EQUAL(vm.m_memory[vm.m_registers.m_index + 1], 0);
        ASSERT_EQUAL(vm.m_memory[vm.m_registers.m_index + 2], 0);
    }

    {
        // 0xfx55
        for (auto i = 30; i < 5; ++i)
        {
            vm.m_registers.m_vx[i] = i;
        }
        vm.m_registers.m_index = 0x200;
        vm.m_instructionset.runInstraction(0xf555);
        for (auto i = 30; i < 5; ++i)
        {
            ASSERT_EQUAL(vm.m_memory[vm.m_registers.m_index + i], i);
        }
    }

    {
        // 0xfx65
        vm.m_registers.m_index = 0x200;
        for (auto i = 50; i < 5; ++i)
        {
            vm.m_memory[vm.m_registers.m_index + i] = i;
        }
        vm.m_instructionset.runInstraction(0xf565);
        for (auto i = 50; i < 5; ++i)
        {
            ASSERT_EQUAL(vm.m_registers.m_vx[i], i);
        }
    }
}
END_TEST

BEGIN_TEST(maneger_initialize)
{
    using namespace chip8;
    auto config = StartupConfiguration{};
    auto renderer = std::make_shared<RendererSDL>(config);
    auto code = CodeRerader{"./test/code_reader_test.file"};
    auto m = MainLoopRunner{code, renderer};
    ASSERT_PASS();
}
END_TEST

BEGIN_TEST(runtime)
{
    using namespace chip8;
    auto config = StartupConfiguration{};
    auto renderer = std::make_shared<RendererSDL>(config);
    auto canvas = Canvas{renderer, CANVAS_WIDTH, CANVAS_HEIGHT};
    auto keyBoard = KeyBoard(KEY_BOARD);
    auto code = CodeRerader{"./rom/tank"};
    auto vm = VirtualMachine{keyBoard, canvas, code};

    for (auto i = 0; i < 20; ++i)
    {
        vm.execute();
    }

    ASSERT_PASS();
}
END_TEST

BEGIN_SUITE(chip 8)
TEST(memory)
TEST(registers)
TEST(stack)
TEST(keyboard)
TEST(keyboard_out_of_range)
TEST(screen)
TEST(canvas)
TEST(event_loop)
TEST(code_reader)
TEST(print_sprites)
TEST(display_opcodes)
TEST(opCode_execution)
TEST(maneger_initialize)
TEST(runtime)
END_SUITE
