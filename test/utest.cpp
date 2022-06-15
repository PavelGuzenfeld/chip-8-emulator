#include "mu_test.h"
#include "settings.hpp"

BEGIN_TEST(memory)
{
    using namespace chip8;
    auto m = Memory();

    for(U16Bit i = 0; i < MEMORY_CAPACITY; ++i)
    {
        m[i] = i % 256;
        TRACER << "m[i] = " << std::to_string(m[i]) << "\n";
    }

    for(u_short i = 0; i < MEMORY_CAPACITY; ++i)
    {
        ASSERT_EQUAL(m[i], i % 256);
    }

}   
END_TEST

BEGIN_SUITE(chip 8)
    TEST(memory)
END_SUITE