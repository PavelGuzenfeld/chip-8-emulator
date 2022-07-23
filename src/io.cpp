#include <iostream>
#include <SDL2/SDL.h>
#include <stdexcept>
#include "io.hpp"

namespace chip8
{

    KeysMap const KEY_BOARD = {
        {SDLK_0, 0},
        {SDLK_1, 1},
        {SDLK_2, 2},
        {SDLK_3, 3},
        {SDLK_4, 4},
        {SDLK_5, 5},
        {SDLK_6, 6},
        {SDLK_7, 7},
        {SDLK_8, 8},
        {SDLK_9, 9},
        {SDLK_a, 10},
        {SDLK_b, 11},
        {SDLK_c, 12},
        {SDLK_d, 13},
        {SDLK_e, 14},
        {SDLK_f, 15},
    };

    static auto initScreen(int width, int height)
    {
        SDL_Init(SDL_INIT_EVERYTHING);
        return SDL_CreateWindow(
            "Chip8 Window",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            width, height, SDL_WINDOW_SHOWN);
    }

    Screen::Screen(U8Bit a_width, U8Bit a_height, U8Bit a_scale)
        : m_screen{initScreen(a_width * a_scale, a_height * a_scale)}
    {
    }

    Screen::~Screen()
    {
        SDL_DestroyWindow(static_cast<SDL_Window *>(m_screen));
    }

    void *Screen::internal()
    {
        return m_screen;
    }

    Renderer::Renderer(Screen &a_screen, U8Bit a_scale, Color a_back, Color a_fore)
        : m_renderer{SDL_CreateRenderer(static_cast<SDL_Window *>(a_screen.internal()), -1, SDL_TEXTUREACCESS_TARGET)}, m_scale(a_scale), m_back(a_back), m_fore(a_fore)
    {
        clear();
    }

    Renderer::~Renderer()
    {
        SDL_DestroyRenderer(static_cast<SDL_Renderer *>(m_renderer));
    }

    void Renderer::setPixel(U8Bit a_x, U8Bit a_y)
    {
        SDL_SetRenderDrawColor(static_cast<SDL_Renderer *>(m_renderer),
                               m_fore.r, m_fore.g, m_fore.b, m_fore.a);
        drawPixel(a_x, a_y);
    }

    void Renderer::resetPixel(U8Bit a_x, U8Bit a_y)
    {
        SDL_SetRenderDrawColor(static_cast<SDL_Renderer *>(m_renderer),
                               m_back.r, m_back.g, m_back.b, m_back.a);
        drawPixel(a_x, a_y);
    }

    void Renderer::clear()
    {
        SDL_SetRenderDrawColor(static_cast<SDL_Renderer *>(m_renderer),
                               m_back.r, m_back.g, m_back.b, m_back.a);
        SDL_RenderClear(static_cast<SDL_Renderer *>(m_renderer));
        present();
    }

    void Renderer::present()
    {
        SDL_RenderPresent(static_cast<SDL_Renderer *>(m_renderer));
    }

    void Renderer::beep(U16Bit a_freq, U16Bit a_duration)
    {
        std::cout << '\a';
        // https://stackoverflow.com/questions/4060601/make-sounds-beep-with-c
    }

    void Renderer::drawPixel(U8Bit a_x, U8Bit a_y)
    {
        SDL_Rect r = {a_x * m_scale, a_y * m_scale, m_scale, m_scale};
        SDL_RenderFillRect(static_cast<SDL_Renderer *>(m_renderer), &r);
    }

    EventLoop::EventLoop(OnKey a_keyDown, OnKey a_keyUp, bool a_testMode)
        : m_keyDown(a_keyDown), m_keyUp(a_keyUp), m_testMode(a_testMode)
    {
    }

    void EventLoop::exitProgram()
    {
        SDL_Quit();
        exit(0);
    }

    void EventLoop::operator()()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event) || m_testMode)
        {
            switch (event.type)
            {
            case SDL_QUIT:
            {
                if (m_testMode)
                {
                    return;
                }
                exitProgram();
            }

            case SDL_KEYDOWN:
            {
                m_keyDown(event.key.keysym.sym);
                break;
            }

            case SDL_KEYUP:
            {
                m_keyUp(event.key.keysym.sym);
                break;
            }
            }
        }
    }

    void simulateKeyEvent(int a_eventType, int a_key)
    {
        SDL_Event sdlevent = {};
        sdlevent.type = a_eventType;
        sdlevent.key.keysym.sym = a_key;
        SDL_PushEvent(&sdlevent);
    }

    void simulateExit()
    {
        SDL_Event sdlevent = {SDL_QUIT};
        SDL_PushEvent(&sdlevent);
    }

    static U8Bit emulatedKey(SDL_Keycode a_key)
    try
    {
        return KEY_BOARD.at(a_key);
    }
    catch (std::out_of_range &e)
    {
        return U8Bit(-1);
    }

    OnKeyDown waitForKeyPressFactory(OnKey a_keyDown)
    {
        return [&]()
        {
            SDL_Event event;
            while (true)
            {
                while (SDL_WaitEvent(&event))
                {
                    if (event.type == SDL_KEYDOWN)
                    {
                        auto c = event.key.keysym.sym;
                        a_keyDown(c);
                        return emulatedKey(c);
                    }

                    if (event.type == SDL_QUIT)
                    {
                        SDL_Quit();
                        exit(0);
                    }
                }
            }
        };
    }

} // namespace chip8