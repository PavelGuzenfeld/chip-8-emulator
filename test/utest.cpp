#include "lib/mu_test.h"
#include <thread>
#include "settings.hpp"
#include "registers.hpp"
#include "keyboard.hpp"
#include "io.hpp"
#include "vm.hpp"
#include "canvas.hpp"
#include "maneger.hpp"

BEGIN_TEST(memory)
{
    using namespace chip8;
    auto m = Memory();
    for(u_short i = 0; i < MEMORY_CAPACITY; ++i)
    {
        ASSERT_EQUAL(m[i], 0);
    }

    for(U16Bit i = 0; i < MEMORY_CAPACITY; ++i)
    {
        m[i] = i % 256;
        //TRACER << "m[i] = " << std::to_string(m[i]) << "\n";
    }

    for(u_short i = 0; i < MEMORY_CAPACITY; ++i)
    {
        ASSERT_EQUAL(m[i], i % 256);
    }

}   
END_TEST

BEGIN_TEST(registers)
{
    using namespace chip8;
    auto regs = Registers{};
    for(U16Bit i = 0; i < VX_CAPACITY; ++i)
    {
        ASSERT_EQUAL(regs.m_vx[i], 0);
    }
    ASSERT_EQUAL(regs.m_index, 0);
    ASSERT_EQUAL(regs.m_PC, 0);
    ASSERT_EQUAL(regs.m_delayTimer, 0);
    ASSERT_EQUAL(regs.m_soundTimer, 0);

    for(U16Bit i = 0; i < VX_CAPACITY; ++i)
    {
        regs.m_vx[i] = i;
    }
    for(U16Bit i = 0; i < VX_CAPACITY; ++i)
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
    for(U16Bit i = 0; i < STACK_CAPACITY; ++i)
    {
        stack.push(i);
    }
    for(U16Bit i = 0; i < VX_CAPACITY; ++i)
    {
        ASSERT_EQUAL(stack.pop(), STACK_CAPACITY-i-1);
    }

}
END_TEST

BEGIN_TEST(keyboard)
{
    using namespace chip8;
    auto keyBoard = KeyBoard(KEY_BOARD);
    for(U16Bit key = 0; key < KEYS_CAPACITY; ++key)
    {
        ASSERT_EQUAL(keyBoard.isVirtualKeyDown(key), false);
    }

    for(auto pairKey: KEY_BOARD)
    {
        auto key = pairKey.first;
        keyBoard.keyDown(key);
    }
    for(U16Bit key = 0; key < KEYS_CAPACITY; ++key)
    {
        ASSERT_EQUAL(keyBoard.isVirtualKeyDown(key), true);
    }

    for(auto pairKey: KEY_BOARD)
    {
        auto key = pairKey.first;
        keyBoard.keyUp(key);
    }
    for(U16Bit key = 0; key < KEYS_CAPACITY; ++key)
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
    auto screen = Screen{CANVAS_WIDTH, CANVAS_HEIGHT, CANVAS_SCALE};
    auto renderer = Renderer{screen, CANVAS_SCALE, BLACK, WHITE};
    
    renderer.clear();
    for(U8Bit x = 0; x < CANVAS_WIDTH; ++x)
    {
        for(U8Bit y = 0; y < CANVAS_HEIGHT; ++y)
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
    auto screen = Screen{CANVAS_WIDTH, CANVAS_HEIGHT, CANVAS_SCALE};
    auto renderer = Renderer{screen, CANVAS_SCALE, BLACK, WHITE};
    auto canvas = Canvas{renderer, CANVAS_WIDTH, CANVAS_HEIGHT};

    for(U8Bit y = 0; y < CANVAS_HEIGHT; ++y)
    {
        for(U8Bit x = 0; x < CANVAS_WIDTH; ++x)
        {
            ASSERT_EQUAL(canvas.isPixelOn(x, y), false);
            auto wasTrue = canvas.setPixel(x, y);
            ASSERT_EQUAL(canvas.isPixelOn(x, y), true);
            ASSERT_EQUAL(wasTrue, false);
        }
    }

    for(U8Bit y = 0; y < CANVAS_HEIGHT; ++y)
    {
        for(U8Bit x = 0; x < CANVAS_WIDTH; ++x)
        {
            ASSERT_EQUAL(canvas.isPixelOn(x, y), true);
            auto wasTrue = canvas.setPixel(x, y);
            ASSERT_EQUAL(canvas.isPixelOn(x, y), false);
            ASSERT_EQUAL(wasTrue, true);
        }
    }
}
END_TEST

// BEGIN_TEST(vm_initialize)
// {
//     using namespace chip8;
//     auto screen = Screen{CANVAS_WIDTH, CANVAS_HEIGHT, CANVAS_SCALE};
//     auto renderer = Renderer{screen, CANVAS_SCALE, BLACK, WHITE};
//     auto canvas = Canvas{renderer, CANVAS_WIDTH, CANVAS_HEIGHT};
//     auto keyBoard = KeyBoard(KEY_BOARD);
//     auto vm = VirtualMachine{keyBoard, canvas};
//     ASSERT_PASS();
// }
// END_TEST

// BEGIN_TEST(maneger_initialize)
// {
//     using namespace chip8;
//     auto m = Maneger{};
//     ASSERT_PASS();
// }
// END_TEST

BEGIN_TEST(event_loop)
{
    using namespace chip8;
    
    auto keyBoard = KeyBoard{KEY_BOARD};
    auto loop = EventLoop{[&](U8Bit a_key){keyBoard.keyDown(a_key);},
                [&](U8Bit a_key){keyBoard.keyUp(a_key);}, true};
    for(U8Bit key = 0; key < 16; ++key)
    {
        ASSERT_EQUAL(keyBoard.isVirtualKeyDown(key), false);
    }

    {
        auto job = [&](){loop();};
        auto worker = std::thread(job);
        for(auto key: KEY_BOARD)
        {
            simulateKeyEvent(SDL_KEYDOWN, static_cast<SDL_EventType>(key.first));
        }
        simulateExit();
        worker.join();

        for(U8Bit key = 0; key < 16; ++key)
        {
            ASSERT_EQUAL(keyBoard.isVirtualKeyDown(key), true);
        }
    }

    {
        auto job = [&](){loop();};
        auto worker = std::thread(job);
        for(auto key: KEY_BOARD)
        {
            simulateKeyEvent(SDL_KEYUP, static_cast<SDL_EventType>(key.first));
        }
        simulateExit();
        worker.join();

        for(U8Bit key = 0; key < 16; ++key)
        {
            ASSERT_EQUAL(keyBoard.isVirtualKeyDown(key), false);
        }
    }
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
    //TEST(vm_initialize)
    //TEST(maneger_initialize)
    TEST(event_loop)
END_SUITE

//TODO: Test printing sprites
//TODO: Seporate cpp form hpp
//TODO: reader test
