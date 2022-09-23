#include <thread> // std::this_thread::sleep_for
#include "delay_functions.hpp"

namespace chip8
{
    void delay(U8Bit &a_delayTimer)
    {
        if (a_delayTimer > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(REFRESH_RATE_MS));
            --a_delayTimer;
        }
    }
} // namespace chip8