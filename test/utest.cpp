#include "lib/mu_test.h"
#include "settings.hpp"
#include "registers.hpp"
#include "keyboard.hpp"
#include "io.hpp"
#include "vm.hpp"

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

BEGIN_TEST(vm_initialize)
{
    using namespace chip8;
    auto vm = VirtualMachine{KEY_BOARD};
    ASSERT_PASS();
}
END_TEST

BEGIN_SUITE(chip 8)
    TEST(memory)
    TEST(registers)
    TEST(stack)
    TEST(keyboard)
    TEST(keyboard_out_of_range)
    TEST(vm_initialize)
END_SUITE