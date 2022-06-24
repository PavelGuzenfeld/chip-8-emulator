#include <iostream>
#include "io.hpp"
#include "vm.hpp"

int main(int argc, char** argv)
{   
    if(argc < 2)
    {
        std::count << "You must provide a file to load\n";
        return -1;
    }
    
    using namespace chip8;
    auto window = Canvas{460, 320};
    auto renderer = Renderer{window};
    renderer.drawRect();
    renderer.present();

    loop();
    
    return 0;
}
