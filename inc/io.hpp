#ifndef IO_HPP
#define IO_HPP

#include <SDL2/SDL.h>
#include <functional>
#include "settings.hpp"

namespace chip8
{

auto const KEY_BOARD = KeysMap{
    {SDLK_0, 0}, {SDLK_1, 1}, {SDLK_2, 2}, {SDLK_3, 3},
    {SDLK_4, 4}, {SDLK_5, 5}, {SDLK_6, 6}, {SDLK_7, 7},
    {SDLK_8, 8}, {SDLK_9, 9}, {SDLK_a, 10}, {SDLK_b, 11},
    {SDLK_c, 12}, {SDLK_d, 13}, {SDLK_e, 14}, {SDLK_f, 15},
    };

static auto initScreen(int width, int height)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    return SDL_CreateWindow(
        "Chip8 Window", 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED,
        width, height, SDL_WINDOW_SHOWN
        );
}

class Screen
{
public:
    Screen(U8Bit a_width, U8Bit a_height, U8Bit a_scale)
    :   m_screen{initScreen(a_width * a_scale, a_height * a_scale)}
    {

    }

    ~Screen()
    {
        SDL_DestroyWindow(m_screen);
    }

    operator SDL_Window*()
    {
        return m_screen;
    }

private:
    SDL_Window* m_screen;
};

class Renderer
{
public:
    Renderer(Screen& a_screen, U8Bit a_scale, Color a_back, Color a_fore)
    :   m_renderer{SDL_CreateRenderer(a_screen, -1, SDL_TEXTUREACCESS_TARGET)}
    ,   m_scale(a_scale)
    ,   m_back(a_back)
    ,   m_fore(a_fore)
    {
        clear();
    }

    ~Renderer()
    {
        SDL_DestroyRenderer(m_renderer);
    }

    void setPixel(U8Bit a_x, U8Bit a_y)
    {
        SDL_SetRenderDrawColor(m_renderer, m_fore.r, m_fore.g, m_fore.b, m_fore.a);
        drawPixel(a_x, a_y);
    }

    void resetPixel(U8Bit a_x, U8Bit a_y)
    {
        SDL_SetRenderDrawColor(m_renderer, m_back.r, m_back.g, m_back.b, m_back.a);
        drawPixel(a_x, a_y);
    }

    void clear()
    {
        SDL_SetRenderDrawColor(m_renderer, m_back.r, m_back.g, m_back.b, m_back.a);
        SDL_RenderClear(m_renderer);
        present();
    }

    void present()
    {
        SDL_RenderPresent(m_renderer);
    }

private:
    void drawPixel(U8Bit a_x, U8Bit a_y)
    {
        SDL_Rect r = {a_x * m_scale, a_y * m_scale, m_scale, m_scale};
        SDL_RenderFillRect(m_renderer, &r);
    }

private:
    SDL_Renderer* m_renderer;
    U8Bit m_scale;
    Color m_back;
    Color m_fore;
};

class EventLoop
{
public:
    using OnKey = std::function<void(U8Bit)>;

    EventLoop(OnKey a_keyDown, OnKey a_keyUp)
    :   m_keyDown(a_keyDown)
    ,   m_keyUp(a_keyUp)
    {

    }

    void operator()()
    {
        while(true)
        {
            SDL_Event event;
            while(SDL_PollEvent(&event))
            {
                switch(event.type)
                {
                    case SDL_QUIT:
                    {
                        return;
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
    }

private:
    OnKey m_keyDown;
    OnKey m_keyUp;

};

}   //namespace chip8

#endif // IO_HPP