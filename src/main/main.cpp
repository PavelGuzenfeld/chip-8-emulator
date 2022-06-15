#include <iostream>
#include "io.hpp"
#include "vm.hpp"

int main(int argc, char** argv)
{   
    using namespace chip8;
    auto window = Canvas{460, 320};
    auto renderer = Renderer{window};
    renderer.drawRect();
    renderer.present();

    loop();
    
    return 0;
}
