#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "engine/Renderer2d.h"

void draw()
{
    Renderer2d::Background(51);
    glm::vec2 size(10, 10);
    for (int x = 0; x < 200; x += 10)
    {
        for (int y = 0; y < 200; y += 10)
        {
            Renderer2d::SetDrawColor(x, x, y);
            glm::vec2 pos(x, y);
            Renderer2d::Rect(pos, size);
        }
    }
}

int main(void)
{
    std::cout << "Initialised -1" << std::endl;

    {
        std::cout << "Initialised -2" << std::endl;

        if (!Renderer2d::InitContext(960, 540))
            return -1;
        std::cout << "Initialised -3" << std::endl;

        Renderer2d::DrawLoop(draw); //this line halts the program since it contains a while loop that runs until the window is closed
    }
    return 0;
}