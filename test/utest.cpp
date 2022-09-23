#include "mu_test.h"
#include <thread>
#include <SDL2/SDL.h>
#include <unistd.h> // sleep()

#define private public
#include "settings.hpp"
#include "registers.hpp"
#include "keyboard.hpp"
#include "io.hpp"
#include "canvas.hpp"
#include "main_loop_runner.hpp"
#include "code_reader.hpp"
#include "memory_initializer.hpp"
#include "delay_functions.hpp"

BEGIN_TEST(memory)
{
    using namespace chip8;
    auto m = Memory{};
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

BEGIN_TEST(memory_initializer)
{
    using namespace chip8;
    auto m = MemoryInitializer::make();

    auto values = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80, // F
    };

    for (auto b = std::begin(values); b != std::end(values); ++b)
    {
        ASSERT_EQUAL(m[b - std::begin(values)], *b);
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
    ASSERT_EQUAL(regs.m_PC, CODE_LOAD_ADDRESS);
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
    auto loop = EventLoopSDL{[&](U8Bit a_key)
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
    auto code = CodeReader{"./test/code_reader_test.file"};
    // TRACER << code.buffer << "\n";
    ASSERT_EQUAL_STR(code.buffer, "hello world!");
}
END_TEST

BEGIN_TEST(load_code_to_memory)
{
    using namespace chip8;
    auto code = CodeReader{"./test/code_reader_test.file"};
    auto memory = Memory{};
    MemoryInitializer::loadCode(memory, code);
    ASSERT_EQUAL(memory[0x200], 0x68);
    ASSERT_EQUAL(memory[0x201], 0x65);
    ASSERT_EQUAL(memory[0x202], 0x6C);
    ASSERT_EQUAL(memory[0x203], 0x6C);
    ASSERT_EQUAL(memory[0x204], 0x6F);
    ASSERT_EQUAL(memory[0x205], 0x20);
    ASSERT_EQUAL(memory[0x206], 0x77);
    ASSERT_EQUAL(memory[0x207], 0x6F);
    ASSERT_EQUAL(memory[0x208], 0x72);
    ASSERT_EQUAL(memory[0x209], 0x6C);
    ASSERT_EQUAL(memory[0x20A], 0x64);
    ASSERT_EQUAL(memory[0x20B], 0x21);
}
END_TEST

BEGIN_TEST(print_sprites)
{
    using namespace chip8;
    auto config = StartupConfiguration{};
    auto renderer = std::make_shared<RendererSDL>(config);
    auto canvas = Canvas{renderer, CANVAS_WIDTH, CANVAS_HEIGHT};
    auto keyBoard = KeyBoard(KEY_BOARD);
    auto memory = MemoryInitializer::make();
    auto callStack = CallStack{};
    auto registers = Registers{};
    auto bus = Bus{memory, callStack, registers, keyBoard, canvas};
    auto instructionset = Instructionset{bus};
    auto x = 0;
    for (U8Bit i = 0; i < 0x10; i += 4)
    {
        instructionset.drawSprite(x, 0, i, 5);
        x += 5;
        sleep(1);
    }

    for (U8Bit i = 0; i < 0x10; ++i)
    {
        instructionset.drawSprite(61, 31, i, 5);
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
    auto memory = MemoryInitializer::make();
    auto callStack = CallStack{};
    auto registers = Registers{};
    auto bus = Bus{memory, callStack, registers, keyBoard, canvas};
    auto instructionset = Instructionset{bus};

    // 0xdfff opcode
    auto const OPCODE = 0xdab5;
    {
        registers.m_vx[0xa] = 0;
        registers.m_vx[0xb] = 0;
        registers.m_index = 0;
        for (auto i = 0x0; i < 0xa; ++i)
        {
            instructionset.runInstraction(OPCODE);
            registers.m_index += 5;
            registers.m_vx[0xa] += 5;
            ASSERT_EQUAL(registers.m_vx[0xf], 0);
        }
        registers.m_vx[0xb] = 10;
        for (auto i = 0xa; i < 0x10; ++i)
        {
            instructionset.runInstraction(OPCODE);
            registers.m_index += 5;
            registers.m_vx[0xa] += 5;
            ASSERT_EQUAL(registers.m_vx[0xf], 0);
        }
    }
    {
        registers.m_vx[0xa] = 0;
        registers.m_vx[0xb] = 0;
        registers.m_index = 0;
        for (auto i = 0x0; i < 0xa; ++i)
        {
            instructionset.runInstraction(OPCODE);
            registers.m_index += 5;
            registers.m_vx[0xa] += 5;
            ASSERT_EQUAL(registers.m_vx[0xf], 1);
        }
        registers.m_vx[0xb] = 10;
        for (auto i = 0xa; i < 0x10; ++i)
        {
            instructionset.runInstraction(OPCODE);
            registers.m_index += 5;
            registers.m_vx[0xa] += 5;
            ASSERT_EQUAL(registers.m_vx[0xf], 1);
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
    auto memory = MemoryInitializer::make();
    auto callStack = CallStack{};
    auto registers = Registers{};
    auto bus = Bus{memory, callStack, registers, keyBoard, canvas};
    auto instructionset = Instructionset{bus};
    {
        // 0x00ee
        callStack.push(0xaa);
        instructionset.runInstraction(0x00ee);
        ASSERT_EQUAL(registers.m_PC, 0xaa);
    }

    {
        // ox1nnn
        instructionset.runInstraction(0x1aaa);
        ASSERT_EQUAL(registers.m_PC, 0xaaa);
    }

    {
        // 0x2nnn
        registers.m_PC = 0xaaa;
        instructionset.runInstraction(0x2bbb);
        ASSERT_EQUAL(registers.m_PC, 0xbbb);
        ASSERT_EQUAL(callStack.pop(), 0xaaa);
    }

    {
        // 0X3xnn
        registers.m_PC = 0;
        registers.m_vx[0xa] = 0xbb;
        instructionset.runInstraction(0x3abb);
        ASSERT_EQUAL(registers.m_PC, 2);
    }

    {
        // 0x4xnn
        registers.m_PC = 0;
        registers.m_vx[0xa] = 0xbb;
        instructionset.runInstraction(0x4abb);
        ASSERT_EQUAL(registers.m_PC, 0);
    }

    {
        // 0x5xy0
        registers.m_PC = 0;
        registers.m_vx[0xa] = 0xbb;
        registers.m_vx[0xb] = 0xbb;
        instructionset.runInstraction(0x5ab0);
        ASSERT_EQUAL(registers.m_PC, 2);
    }

    {
        // 0x6xnn
        instructionset.runInstraction(0x6abb);
        ASSERT_EQUAL(registers.m_vx[0xa], 0xbb);
    }

    {
        // 0x7xnn
        registers.m_vx[0xa] = 0x1;
        instructionset.runInstraction(0x7abb);
        ASSERT_EQUAL(registers.m_vx[0xa], 0xbc);
    }

    {
        // 0x8xy0
        registers.m_vx[0xa] = 0x1;
        registers.m_vx[0xb] = 0x55;
        instructionset.runInstraction(0x8ab0);
        ASSERT_EQUAL(registers.m_vx[0xa], 0x55);
    }

    {
        // 0x8xy1
        registers.m_vx[0xa] = 0x0;
        registers.m_vx[0xb] = 0x1;
        instructionset.runInstraction(0x8ab1);
        ASSERT_EQUAL(registers.m_vx[0xa], 0x1);
    }

    {
        // 0x8xy2
        registers.m_vx[0xa] = 0x0f;
        registers.m_vx[0xb] = 0x03;
        instructionset.runInstraction(0x8ab2);
        ASSERT_EQUAL(registers.m_vx[0xa], 0x03);
    }

    {
        // 0x8xy3
        registers.m_vx[0xa] = 0x0f;
        registers.m_vx[0xb] = 0x03;
        instructionset.runInstraction(0x8ab3);
        ASSERT_EQUAL(registers.m_vx[0xa], 0x0c);
    }

    {
        // 0x8xy4
        registers.m_vx[0xa] = 0xff;
        registers.m_vx[0xb] = 0x03;
        registers.m_vx[0xf] = 0;
        instructionset.runInstraction(0x8ab4);
        ASSERT_EQUAL(registers.m_vx[0xa], static_cast<U8Bit>(0xff + 0x03));
        ASSERT_EQUAL(registers.m_vx[0xf], 1);
    }

    {
        // 0x8xy4
        registers.m_vx[0xa] = 0x04;
        registers.m_vx[0xb] = 0x03;
        registers.m_vx[0xf] = 0;
        instructionset.runInstraction(0x8ab4);
        ASSERT_EQUAL(registers.m_vx[0xa], static_cast<U8Bit>(0x04 + 0x03));
        ASSERT_EQUAL(registers.m_vx[0xf], 0);
    }

    {
        // 0x8xy5
        registers.m_vx[0xa] = 0x04;
        registers.m_vx[0xb] = 0x03;
        registers.m_vx[0xf] = 0;
        instructionset.runInstraction(0x8ab5);
        ASSERT_EQUAL(registers.m_vx[0xa], static_cast<U8Bit>(0x04 - 0x03));
        ASSERT_EQUAL(registers.m_vx[0xf], 0);
    }

    {
        // 0x8xy5
        registers.m_vx[0xa] = 0x03;
        registers.m_vx[0xb] = 0x04;
        registers.m_vx[0xf] = 0;
        instructionset.runInstraction(0x8ab5);
        ASSERT_EQUAL(registers.m_vx[0xa], static_cast<U8Bit>(0x03 - 0x04));
        ASSERT_EQUAL(registers.m_vx[0xf], 1);
    }

    {
        // 0x8xy6
        registers.m_vx[0xa] = 0x03;
        registers.m_vx[0xb] = 0x04;
        registers.m_vx[0xf] = 0;
        instructionset.runInstraction(0x8ab6);
        ASSERT_EQUAL(registers.m_vx[0xa], static_cast<U8Bit>(0x03 >> 1));
        ASSERT_EQUAL(registers.m_vx[0xf], 1);
    }

    {
        // 0x8xy6
        registers.m_vx[0xa] = 0x02;
        registers.m_vx[0xb] = 0x04;
        registers.m_vx[0xf] = 0;
        instructionset.runInstraction(0x8ab6);
        ASSERT_EQUAL(registers.m_vx[0xa], static_cast<U8Bit>(0x02 >> 1));
        ASSERT_EQUAL(registers.m_vx[0xf], 0);
    }

    {
        // 0x8xy7
        registers.m_vx[0xa] = 0x03;
        registers.m_vx[0xb] = 0x04;
        registers.m_vx[0xf] = 0;
        instructionset.runInstraction(0x8ab7);
        ASSERT_EQUAL(registers.m_vx[0xa], static_cast<U8Bit>(0x04 - 0x03));
        ASSERT_EQUAL(registers.m_vx[0xf], 1);
    }

    {
        // 0x8xy7
        registers.m_vx[0xa] = 0x04;
        registers.m_vx[0xb] = 0x03;
        registers.m_vx[0xf] = 0;
        instructionset.runInstraction(0x8ab7);
        ASSERT_EQUAL(registers.m_vx[0xa], static_cast<U8Bit>(0x03 - 0x04));
        ASSERT_EQUAL(registers.m_vx[0xf], 0);
    }

    {
        // 0x8xyE
        registers.m_vx[0xa] = 0x0ff;
        registers.m_vx[0xb] = 0x04;
        registers.m_vx[0xf] = 0;
        instructionset.runInstraction(0x8abe);
        ASSERT_EQUAL(registers.m_vx[0xa], static_cast<U8Bit>(0x0ff << 1));
        ASSERT_EQUAL(registers.m_vx[0xf], 1);
    }

    {
        // 0x8xyE
        registers.m_vx[0xa] = 0x02;
        registers.m_vx[0xb] = 0x04;
        registers.m_vx[0xf] = 0;
        instructionset.runInstraction(0x8abe);
        ASSERT_EQUAL(registers.m_vx[0xa], static_cast<U8Bit>(0x02 << 1));
        ASSERT_EQUAL(registers.m_vx[0xf], 0);
    }

    {
        // 0x9xy0
        registers.m_vx[0xa] = 0x02;
        registers.m_vx[0xb] = 0x04;
        registers.m_vx[0xf] = 0;
        instructionset.runInstraction(0x9ab0);
        ASSERT_EQUAL(registers.m_vx[0xf], 0);
    }

    {
        // 0xafff
        registers.m_index = 0;
        instructionset.runInstraction(0xaaaa);
        ASSERT_EQUAL(registers.m_index, 0xaaa);
    }

    {
        // 0xbfff
        registers.m_vx[0] = 5;
        instructionset.runInstraction(0xbaaa);
        ASSERT_EQUAL(registers.m_PC, 5 + 0xaaa);
    }

    {
        // 0xcfff
        auto sum = 0;
        for (auto i = 0; i < 10000; ++i)
        {
            instructionset.runInstraction(0xcfff);
            sum += registers.m_vx[0xf];
        }
        ASSERT_EQUAL(static_cast<int>(sum / 10000), int(255 / 2));
    }

    {
        // 0xex9e
        registers.m_vx[0] = 0x01;
        registers.m_PC = 0x200;
        keyBoard.keyDown(SDLK_2);
        instructionset.runInstraction(0xe09e);
        ASSERT_EQUAL(registers.m_PC, 0x200);
    }

    {
        // 0xex9e
        registers.m_vx[0] = 0x02;
        registers.m_PC = 0x200;
        keyBoard.keyDown(SDLK_2);
        instructionset.runInstraction(0xe09e);
        ASSERT_EQUAL(registers.m_PC, 0x202);
    }

    {
        // 0xexa1
        registers.m_vx[0] = 0x01;
        registers.m_PC = 0x200;
        keyBoard.keyDown(SDLK_2);
        instructionset.runInstraction(0xe0a1);
        ASSERT_EQUAL(registers.m_PC, 0x202);
    }

    {
        // 0xexa1
        registers.m_vx[0] = 0x02;
        registers.m_PC = 0x200;
        keyBoard.keyDown(SDLK_2);
        instructionset.runInstraction(0xe0a1);
        ASSERT_EQUAL(registers.m_PC, 0x200);
    }

    {
        // 0xfx07
        registers.m_vx[0] = 0x01;
        registers.m_delayTimer = 0x02;
        instructionset.runInstraction(0xf007);
        ASSERT_EQUAL(registers.m_vx[0], 0x02);
    }

    {
        // 0xfx0a
        registers.m_vx[0] = 0x01;
        auto job = [&]()
        { instructionset.runInstraction(0xf00a); };
        auto worker = std::thread(job);
        simulateKeyEvent(SDL_KEYDOWN, SDLK_2);
        worker.join();
        ASSERT_EQUAL(registers.m_vx[0], 0x02);
    }

    {
        // 0xfx15
        registers.m_vx[0] = 0x01;
        instructionset.runInstraction(0xf015);
        ASSERT_EQUAL(registers.m_delayTimer, 0x01);
    }

    {
        // 0xfx18
        registers.m_vx[0] = 0x01;
        instructionset.runInstraction(0xf018);
        ASSERT_EQUAL(registers.m_soundTimer, 0x01);
    }

    {
        // 0xfx1e
        registers.m_vx[0] = 0x01;
        registers.m_index = 0x02;
        instructionset.runInstraction(0xf01e);
        ASSERT_EQUAL(registers.m_index, 0x03);
    }

    {
        // 0xfx29
        registers.m_vx[0] = 0x01;
        instructionset.runInstraction(0xf029);
        ASSERT_EQUAL(registers.m_index, 0x01 * 5);
    }

    {
        // 0xfx33
        registers.m_vx[0] = 123;
        instructionset.runInstraction(0xf033);
        ASSERT_EQUAL(memory[registers.m_index], 1);
        ASSERT_EQUAL(memory[registers.m_index + 1], 2);
        ASSERT_EQUAL(memory[registers.m_index + 2], 3);
    }

    {
        // 0xfx33
        // case vx = 0x0
        registers.m_vx[0] = 0;
        instructionset.runInstraction(0xf033);
        ASSERT_EQUAL(memory[registers.m_index], 0);
        ASSERT_EQUAL(memory[registers.m_index + 1], 0);
        ASSERT_EQUAL(memory[registers.m_index + 2], 0);
    }

    {
        // 0xfx55
        for (auto i = 30; i < 5; ++i)
        {
            registers.m_vx[i] = i;
        }
        registers.m_index = 0x200;
        instructionset.runInstraction(0xf555);
        for (auto i = 30; i < 5; ++i)
        {
            ASSERT_EQUAL(memory[registers.m_index + i], i);
        }
    }

    {
        // 0xfx65
        registers.m_index = 0x200;
        for (auto i = 50; i < 5; ++i)
        {
            memory[registers.m_index + i] = i;
        }
        instructionset.runInstraction(0xf565);
        for (auto i = 50; i < 5; ++i)
        {
            ASSERT_EQUAL(registers.m_vx[i], i);
        }
    }
}
END_TEST

BEGIN_TEST(delay_functions)
{
    using namespace chip8;
    auto registers = Registers{};
    delay(registers.m_delayTimer);
    beep(registers.m_soundTimer);
    ASSERT_EQUAL(registers.m_delayTimer, 0);
    ASSERT_EQUAL(registers.m_soundTimer, 0);

    registers.m_delayTimer = 3;
    registers.m_soundTimer = 3;
    delay(registers.m_delayTimer);
    beep(registers.m_soundTimer);
    ASSERT_EQUAL(registers.m_delayTimer, 2);
    ASSERT_EQUAL(registers.m_soundTimer, 2);
}
END_TEST

BEGIN_TEST(main_loop_init)
{
    using namespace chip8;
    auto config = StartupConfiguration{};
    auto renderer = std::make_shared<RendererSDL>(config);
    auto canvas = Canvas{renderer, CANVAS_WIDTH, CANVAS_HEIGHT};
    auto keyBoard = KeyBoard(KEY_BOARD);
    auto memory = MemoryInitializer::make();
    auto callStack = CallStack{};
    auto registers = Registers{};
    auto bus = Bus{memory, callStack, registers, keyBoard, canvas};
    auto instructionset = Instructionset{bus};
    auto loop = std::make_shared<EventLoopSDL>([&](U8Bit a_key)
                                               { keyBoard.keyDown(a_key); },
                                               [&](U8Bit a_key)
                                               { keyBoard.keyUp(a_key); });
    auto mainLoop = MainLoopRunner{bus, instructionset, loop};
    ASSERT_PASS();
}
END_TEST

BEGIN_TEST(dry_run)
{
    using namespace chip8;
    auto config = StartupConfiguration{};
    auto renderer = std::make_shared<RendererSDL>(config);
    auto canvas = Canvas{renderer, CANVAS_WIDTH, CANVAS_HEIGHT};
    auto keyBoard = KeyBoard(KEY_BOARD);
    auto code = CodeReader{"./rom/tank"};
    auto memory = MemoryInitializer::make();
    MemoryInitializer::loadCode(memory, code);
    auto callStack = CallStack{};
    auto registers = Registers{};
    auto bus = Bus{memory, callStack, registers, keyBoard, canvas};
    auto instructionset = Instructionset{bus};

    for (auto i = 0; i < 20; ++i)
    {
        instructionset.runNextInstruction();
    }

    ASSERT_PASS();
}
END_TEST

BEGIN_SUITE(chip 8)
TEST(memory)
TEST(memory_initializer)
TEST(registers)
TEST(stack)
TEST(keyboard)
TEST(keyboard_out_of_range)
TEST(screen)
TEST(canvas)
TEST(event_loop)
TEST(code_reader)
TEST(load_code_to_memory)
TEST(print_sprites)
TEST(display_opcodes)
TEST(opCode_execution)
TEST(delay_functions)
TEST(main_loop_init)
TEST(dry_run)
END_SUITE
