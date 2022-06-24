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

class Screen
{
public:
    Screen(U8Bit a_width, U8Bit a_height, U8Bit a_scale);
    ~Screen();

    operator SDL_Window*();

private:
    SDL_Window* m_screen;
};

class Renderer
{
public:
    Renderer(Screen& a_screen, U8Bit a_scale, Color a_back, Color a_fore);
    ~Renderer();

    void setPixel(U8Bit a_x, U8Bit a_y);
    void resetPixel(U8Bit a_x, U8Bit a_y);
    
    void present();
    void clear();
    
    void beep(U16Bit a_freq, U16Bit a_duration);


private:
    void drawPixel(U8Bit a_x, U8Bit a_y);

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

    EventLoop(OnKey a_keyDown, OnKey a_keyUp, bool a_testMode = false);

    void operator()();

    void exitProgram();

private:
    OnKey m_keyDown;
    OnKey m_keyUp;
    bool m_testMode;

};

void simulateKeyEvent(Uint32 a_eventType, int a_key);
void simulateExit();

}   //namespace chip8

#endif // IO_HPP