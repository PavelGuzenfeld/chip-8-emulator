#include <SDL2/SDL.h>

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow(
        "Chip8 Window", 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED,
        460, 320, SDL_WINDOW_SHOWN
    );

    while(1);

    SDL_DestroyWindow(window);
    return 0;
}
