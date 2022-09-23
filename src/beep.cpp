#ifdef _WIN32
#include <Windows.h> //Beep()
#else
#include <chrono>   // std::chrono
#include <iostream> // std::cout
#endif

#include "delay_functions.hpp"

namespace chip8
{
    static void linuxBeep()
    {
        std::cout << "\a" << std::flush;
    }

    void beep(U8Bit &a_soundTimer)
    {
        if (a_soundTimer > 0)
        {
#ifdef _WIN32
            Beep(REFRESH_RATE_MS, SOUND_FREQUENCY);
#else
            auto t_start = std::chrono::high_resolution_clock::now();
            while (true)
            {
                linuxBeep();
                auto t_end = std::chrono::high_resolution_clock::now();
                double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
                if (elapsed_time_ms > REFRESH_RATE_MS)
                {
                    break;
                }
            }
#endif
            --a_soundTimer;
        }
    }

} // namespace chip8