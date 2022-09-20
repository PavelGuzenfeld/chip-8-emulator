#ifndef IO_HPP
#define IO_HPP

#include "settings.hpp"
#include <memory>

namespace chip8
{

    extern KeysMap const KEY_BOARD;

    class Screen
    {
    public:
        Screen(U8Bit a_width, U8Bit a_height, U8Bit a_scale);

        auto *internal();

    private:
        std::shared_ptr<void> m_screen;
    };

    class Renderer
    {
    public:
        Renderer(Screen &a_screen, U8Bit a_scale, Color a_back, Color a_fore);

        void setPixel(U8Bit a_x, U8Bit a_y);
        void resetPixel(U8Bit a_x, U8Bit a_y);

        void present();
        void clear();

    private:
        void drawPixel(U8Bit a_x, U8Bit a_y);

    private:
        std::shared_ptr<void> m_renderer;
        U8Bit m_scale;
        Color m_back;
        Color m_fore;
    };

    class EventLoop
    {
    public:
        EventLoop(OnKey a_keyDown, OnKey a_keyUp, bool a_testMode = false);

        void operator()();

        void exitProgram();

    private:
        OnKey m_keyDown;
        OnKey m_keyUp;
        bool m_testMode;
    };

    OnKeyDown waitForKeyPressFactory(OnKey a_keyDown);

    void simulateKeyEvent(int a_eventType, int a_key);
    void simulateExit();

} // namespace chip8

#endif // IO_HPP
