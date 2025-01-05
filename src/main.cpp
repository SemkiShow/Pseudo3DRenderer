#include "Render.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>

// Public variables init
// std::string brightness = " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";
// std::string brightness = "#";
int* playerPosition;
int windowSize[2] = {16*32*2, 9*32*2};
int verticalSync = true;

int main()
{
    // Local variables init
    sf::RenderWindow window(sf::VideoMode(windowSize[0], windowSize[1]), "Pseudo3DShooter");
    // glEnable(GL_TEXTURE_2D);
    if (verticalSync)
    {
        window.setFramerateLimit(144);
        window.setVerticalSyncEnabled(true);
    }
    else
    {
        window.setFramerateLimit(0);
        window.setVerticalSyncEnabled(false);
    }

    sf::RectangleShape rectangle(sf::Vector2f(0, 0));
    // rectangle.setFillColor(Color(127, 127, 255));
    rectangle.setFillColor(sf::Color(0, 132, 208));
    // rectangle.setFillColor(Color(34, 165, 241));

    sf::Font font;
    font.loadFromFile("../JetBrainsMonoNerdFont-Medium.ttf");
    sf::Text FPS;
    FPS.setFont(font);
    FPS.setString(std::to_string(123));
    FPS.setCharacterSize(24);
    sf::Text FOVtext;
    FOVtext.setPosition(0, 25);
    FOVtext.setFont(font);
    FOVtext.setCharacterSize(24);

    std::vector<std::string> _mapVector;
    std::string mapFileName = "map2";
    std::fstream mapFile("../maps/" + mapFileName + ".txt");
    std::string mapFileContent;
    while (getline(mapFile, mapFileContent))
    {
        // std::cout << mapFileContent << std::endl;
        _mapVector.push_back(mapFileContent);
    }
    mapFile.close();
    int _mapSize = _mapVector.size();
    std::string* _map = new std::string[_mapSize];
    for (int i = 0; i < _mapSize; i++)
    {
        _map[i] = _mapVector[i];
    }
    
    // std::cout << mapFileContent << std::endl;

    // std::string* _map = new std::string[sizeof(map2) / sizeof(*map2)];
    // char (*_map)[sizeof(map2) / sizeof(*map2)] = new char[sizeof(map2) / sizeof(*map2)][map2[0].length()];
    // for (int i = 0; i < sizeof(map2) / sizeof(*map2); i++)
    // {
    //     for (int j = 0; j < map2[i].length(); j++)
    //     {
    //         _map[i][j] = map2[i][j];
    //     }
    // }
    // int _mapSize = sizeof(map2) / sizeof(*map2);

    int _render;

    // Debug info
    std::cout << "<Debug info>" << std::endl;

    // std::cout << brightness << std::endl;
    // std::cout << 3.141592654 << std::endl;  

    // GetTerminalSize(windowSize[0], windowSize[1]);
    std::cout << "Window size is " << windowSize[0] << ", " << windowSize[1] << std::endl;

    playerPosition = GetPlayerPosition(_map, _mapSize);
    // std::cout << _map[0].length() << std::endl;
    std::cout << "Player position: " << playerPosition[0] << ", " << playerPosition[1] << std::endl;
    
    int _angle;
    bool _askForAngle = false;
    if (_askForAngle)
    {
        std::cout << "Enter angle: ";
        std::cin >> _angle;
    }
    else
    {
        _angle = 42.0; // Veritasium angle
    }
    int* _rayDestination = AngleToRayDestination(_angle, playerPosition);
    std::cout << "Ray destination at angle " << _angle << " is " << _rayDestination[0] << ", " << _rayDestination[1] << std::endl;

    int* _rayPosition = RayCollisionDetection(_rayDestination, _map, _mapSize, playerPosition);
    std::cout << "Ray position is " << _rayPosition[0] << ", " << _rayPosition[1] << ". The ray has travelled " << _rayPosition[3] << " m" << std::endl;

    // double* _output = new double[windowSize[0]];
    double* _output = RenderFrame(_map, _mapSize, windowSize, playerPosition);
    // for (int i = 0; i < windowSize[0]; i++)
    // {
    //     std::cout << _output[i] << "; ";
    // }
    // std::cout << std::endl;

    std::cout << "</Debug info>" << std::endl;

    time_t FPSCounter;
    time(&FPSCounter);
    sf::Clock clock;

    // Main loop
    while (window.isOpen())
    {
        clock.restart();

        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear();

        _output = RenderFrame(_map, _mapSize, windowSize, playerPosition);
        for (int i = 0; i < windowSize[0]; i++)
        {
            rectangle.setSize(sf::Vector2f(1, _output[i] * windowSize[1]));
            rectangle.setPosition(i, windowSize[1] - _output[i] * windowSize[1]);
            window.draw(rectangle);
        }

        if (time(NULL) - FPSCounter >= 1)
        {
            FPS.setString("FPS: " + std::to_string((int)(1 / clock.getElapsedTime().asSeconds())));
            time(&FPSCounter);
            // FOV+=5;
            FOVtext.setString("FOV: " + std::to_string(FOV));
        }
        window.draw(FPS);
        window.draw(FOVtext);

        window.display();
        // clock.restart();
    }
}
