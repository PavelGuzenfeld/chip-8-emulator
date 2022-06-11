#ifndef IO_HPP
#define IO_HPP

#include <SDL2/SDL.h>
#include "settings.hpp"

namespace chip8
{

auto const KEY_BOARD = KeysMap{
    {SDLK_0, 0}, {SDLK_1, 1}, {SDLK_2, 2}, {SDLK_3, 3},
    {SDLK_4, 4}, {SDLK_5, 5}, {SDLK_6, 6}, {SDLK_7, 7},
    {SDLK_8, 8}, {SDLK_9, 9}, {SDLK_a, 10}, {SDLK_b, 11},
    };

static auto initCanvas(int width, int height)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    return SDL_CreateWindow(
        "Chip8 Window", 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED,
        width, height, SDL_WINDOW_SHOWN
        );
}

class Canvas
{
public:
    Canvas(int width, int height)
    :   m_canvas{initCanvas(width, height)}
    {

    }

    ~Canvas()
    {
        SDL_DestroyWindow(m_canvas);
    }

    operator SDL_Window*()
    {
        return m_canvas;
    }

private:
    SDL_Window* m_canvas;
    
};

class Renderer
{
public:
    Renderer(Canvas& a_canvas)
    :   m_renderer{SDL_CreateRenderer(a_canvas, -1, SDL_TEXTUREACCESS_TARGET)}
    {
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
        SDL_RenderClear(m_renderer);
    }

    ~Renderer()
    {
        SDL_DestroyRenderer(m_renderer);
    }

    void drawRect()
    {
        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 0);
        SDL_Rect r = {0,0,40,40};
        SDL_RenderFillRect(m_renderer, &r);
    }

    void present()
    {
        SDL_RenderPresent(m_renderer);
    }

private:
    SDL_Renderer* m_renderer;
};

void loop()
{
    while(true)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                return;
            }
        }
    }
}

}   //namespace chip8

#endif // IO_HPP