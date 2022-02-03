#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <time.h>
#include "SFML/include/SFML/Graphics.hpp"
#include "World.h"

std::string current_time() {
    time_t now = time(NULL);
    struct tm tstruct;
    char buf[40];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%X", &tstruct);
    return buf;
}

int main(int argc, char** argv)
{
    World w(76512390423);
    w.generate();


    sf::RenderWindow window(sf::VideoMode(w.dim.x, w.dim.y), "Erosion");

    sf::Image image;
    image.create(w.dim.x, w.dim.x);

    sf::Texture texture;
    texture.loadFromImage(image);

    sf::Sprite output;
    output.setTexture(texture);
    bool checkErosion = false;
    bool flag = 1;
    std::cout << "------------------------------" << std::endl;
    std::cout << "Q: start erosion" << std::endl;
    std::cout << "W: end erosion" << std::endl;
    std::cout << "E: save image" << std::endl;
    std::cout << "R: generate new heightmap" << std::endl;
    std::cout << "F: load heightmap from file (output.png)" << std::endl;
    std::cout << "------------------------------" << std::endl;
    while (window.isOpen()) 
    {

        sf::Event ev;

        while (window.pollEvent(ev)) 
        {
            if (ev.type == sf::Event::Closed) 
            {
                window.close();
            }
            if (ev.type == sf::Event::KeyPressed)
            {
                if (ev.key.code == sf::Keyboard::Q)
                {
                    checkErosion = true;
                }
                if (ev.key.code == sf::Keyboard::W)
                {
                    checkErosion = false;
                }
                if (ev.key.code == sf::Keyboard::E)
                {
                    std::string path = "../heightmap ";
                    path += current_time();
                    path += ".png";
                    for (int i = 0; i < path.size(); i++)
                    {
                        if (path[i] == ':')
                            path[i] = ' ';
                    }
                    image.saveToFile(path);
                }
                if (ev.key.code == sf::Keyboard::R)
                {
                    w.generate();
                    std::cout << "new heightmap generated" << std::endl;
                    flag = 1;
                }
                if (ev.key.code == sf::Keyboard::F)
                {
                    image.loadFromFile("../heightmap.png");
                    for (int x = 0; x < w.dim.x; x++)
                    {
                        for (int y = 0; y < w.dim.x; y++)
                        {
                            w.heightmap[x][y] = (image.getPixel(x, y).r / 255.0);
                        }
                    }
                    std::cout << "heightmap is loaded from a file" << std::endl;
                }
           
            }
        }

        if (checkErosion) w.erode(1000);

        for (int x = 0; x < w.dim.x; x++)
        {
            for (int y = 0; y < w.dim.x; y++)
            {
                sf::Uint8 brightness = (int)(w.heightmap[x][y] * 255.0);
                image.setPixel(x, y, sf::Color(brightness, brightness, brightness));
            }
        }
        if (flag == 1) {
            //image.saveToFile("../start_hm.png");
            flag = 0;
        }
        w.updated = true;
        
        window.clear();
        texture.update(image);
        window.draw(output);
        window.display();
    }
	return 0;
}