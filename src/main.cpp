#include "Render.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>

// Public variables init
// Rendering data
// std::string brightness = " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";
// std::string brightness = "#";
int windowSize[2] = {16*32*2, 9*32*2};
int verticalSync = true;

// Game data
int* playerPosition;
std::vector<std::string> _mapVector;
std::string* _map;
int _mapSize;

int PrintDebugInfo()
{
    // Debug info
    std::cout << "<Debug info>" << std::endl;

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

    double* _output = RenderFrame(_map, _mapSize, windowSize, playerPosition);

    std::cout << "</Debug info>" << std::endl;

    return 0;
}

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
    font.loadFromFile("./JetBrainsMonoNerdFont-Medium.ttf");
    sf::Text FPS;
    FPS.setFont(font);
    FPS.setString(std::to_string(123));
    FPS.setCharacterSize(24);
    sf::Text FOVtext;
    FOVtext.setPosition(0, 25);
    FOVtext.setFont(font);
    FOVtext.setCharacterSize(24);

    std::string mapFileName = "map2";
    std::fstream mapFile("./maps/" + mapFileName + ".txt");
    std::string mapFileContent;
    while (getline(mapFile, mapFileContent))
    {
        // std::cout << mapFileContent << std::endl;
        _mapVector.push_back(mapFileContent);
    }
    mapFile.close();
    _mapSize = _mapVector.size();
    _map = new std::string[_mapSize];
    for (int i = 0; i < _mapSize; i++)
    {
        _map[i] = _mapVector[i];
    }

    PrintDebugInfo();

    time_t FPSCounter;
    time(&FPSCounter);
    sf::Clock clock;
    double* _output;
    double deltaTime;

    // Main loop
    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();

        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        // if (sf::Mouse::getPosition().x != 100)
        // {
        //     rotationXOffset += sf::Mouse::getPosition().x * sensitivity;
        //     if (rotationXOffset >= 360) rotationXOffset = 0;
        //     // std::cout << sf::Mouse::getPosition().x << ", ";
        //     sf::Mouse::setPosition(sf::Vector2i(100, 100));
        // }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            rotationXOffset -= sensitivity * deltaTime;
            if (rotationXOffset >= 360) rotationXOffset = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            rotationXOffset += sensitivity * deltaTime;
            if (rotationXOffset >= 360) rotationXOffset = 0;
        }

        window.clear();

        _output = RenderFrame(_map, _mapSize, windowSize, playerPosition);
        for (int i = 0; i < windowSize[0]; i++)
        {
            rectangle.setSize(sf::Vector2f(1, _output[i] * windowSize[1]));
            rectangle.setPosition(i, windowSize[1] - _output[i] * windowSize[1]);
            window.draw(rectangle);
        }

        if (time(0) - FPSCounter >= 1)
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
