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
double* playerPosition;
std::vector<std::string> _mapVector;
std::string* _map;
int _mapSize;

int PrintDebugInfo()
{
    // Debug info
    std::cout << "<Debug info>" << std::endl;

    std::cout << "Window size is " << windowSize[0] << ", " << windowSize[1] << std::endl;

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

    std::cout << "Enter the map ID" << std::endl;
    std::string mapID;
    std::getline(std::cin, mapID);
    std::fstream mapFile("maps/map" + mapID + ".txt");
    std::string mapFileContent;
    while (getline(mapFile, mapFileContent))
    {
        // std::cout << mapFileContent << std::endl;
        _mapVector.push_back(mapFileContent);
    }
    std::string buf;
    for (int i = 0; i < _mapVector.size(); i++)
    {
        buf = "";
        for (int j = 0; j < _mapVector[0].size(); j++)
        {
            // std::cout << _mapVector[i][j];
            for (int k = 0; k < scale; k++)
            {
                buf += _mapVector[i][j];
            }
        }
        // std::cout << std::endl;
        _mapVector[i] = buf;
        for (int k = 0; k <= scale; k++)
        {
            _mapVector.insert(_mapVector.begin() + i + 1, buf);
            i++;            
        }
    }
    mapFile.close();
    _mapSize = _mapVector.size();
    _map = new std::string[_mapSize];
    for (int i = 0; i < _mapSize; i++)
    {
        _map[i] = _mapVector[i];
    }

    playerPosition = GetPlayerPosition(_map, _mapSize);
    // PrintDebugInfo();

    // Local variables init
    sf::RenderWindow window(sf::VideoMode({windowSize[0], windowSize[1]}), "Pseudo3DShooter");
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

    sf::Font font("assets/JetBrainsMonoNerdFont-Medium.ttf");
    sf::Text FPS(font);
    FPS.setString(std::to_string(123));
    FPS.setCharacterSize(24);
    sf::Text FOVtext(font);
    FOVtext.setPosition({0, 25});
    FOVtext.setCharacterSize(24);

    // time_t FPSCounter;
    // time(&FPSCounter);
    double* _output;
    sf::Clock fpsClock;
    double deltaTime;
    sf::Clock delayClock;

    // Main loop
    while (window.isOpen())
    {
        deltaTime = fpsClock.restart().asSeconds();

        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            rotationXOffset -= rotationSensitivity * deltaTime;
            if (rotationXOffset >= 360) rotationXOffset = 0;
            if (rotationXOffset <= -360) rotationXOffset = 0;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            rotationXOffset += rotationSensitivity * deltaTime;
            if (rotationXOffset >= 360) rotationXOffset = 0;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        {
            playerPosition[0] += cos(DegreesToRadians(rotationXOffset + FOV/2)) * movementSensitivity * deltaTime;
            playerPosition[1] += sin(DegreesToRadians(rotationXOffset)) * movementSensitivity * deltaTime;
            if (_map[(int)playerPosition[1]][(int)playerPosition[0]] == '#')
            {
                playerPosition[0] -= cos(DegreesToRadians(rotationXOffset + FOV/2)) * movementSensitivity * deltaTime * 2;
                playerPosition[1] -= sin(DegreesToRadians(rotationXOffset)) * movementSensitivity * deltaTime * 2;
            }
            if (rotationXOffset >= 360) rotationXOffset = 0;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        {
            playerPosition[0] -= cos(DegreesToRadians(rotationXOffset + FOV/2)) * movementSensitivity * deltaTime;
            playerPosition[1] -= sin(DegreesToRadians(rotationXOffset)) * movementSensitivity * deltaTime;
            if (_map[(int)playerPosition[1]][(int)playerPosition[0]] == '#')
            {
                playerPosition[0] += cos(DegreesToRadians(rotationXOffset + FOV/2)) * movementSensitivity * deltaTime * 2;
                playerPosition[1] += sin(DegreesToRadians(rotationXOffset)) * movementSensitivity * deltaTime * 2;
            }
            if (rotationXOffset >= 360) rotationXOffset = 0;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            playerPosition[0] += cos(DegreesToRadians(rotationXOffset + FOV/2 - 90)) * movementSensitivity * deltaTime;
            playerPosition[1] += sin(DegreesToRadians(rotationXOffset)) * movementSensitivity * deltaTime;
            if (_map[(int)playerPosition[1]][(int)playerPosition[0]] == '#')
            {
                playerPosition[0] -= cos(DegreesToRadians(rotationXOffset + FOV/2 - 90)) * movementSensitivity * deltaTime * 2;
                playerPosition[1] -= sin(DegreesToRadians(rotationXOffset)) * movementSensitivity * deltaTime * 2;
            }
            if (rotationXOffset >= 360) rotationXOffset = 0;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            playerPosition[0] -= cos(DegreesToRadians(rotationXOffset + FOV/2 - 90)) * movementSensitivity * deltaTime;
            playerPosition[1] -= sin(DegreesToRadians(rotationXOffset)) * movementSensitivity * deltaTime;
            if (_map[(int)playerPosition[1]][(int)playerPosition[0]] == '#')
            {
                playerPosition[0] += cos(DegreesToRadians(rotationXOffset + FOV/2 - 90)) * movementSensitivity * deltaTime * 2;
                playerPosition[1] += sin(DegreesToRadians(rotationXOffset)) * movementSensitivity * deltaTime * 2;
            }
            if (rotationXOffset >= 360) rotationXOffset = 0;
        }


        window.clear();

        _output = RenderFrame(_map, _mapSize, windowSize, playerPosition);
        for (int i = 0; i < windowSize[0]; i++)
        {
            double wallSize = _output[i] * windowSize[1] - (windowSize[1] - _output[i] * windowSize[1]);
            rectangle.setSize(sf::Vector2f(1, wallSize > 0 ? wallSize : 0));
            rectangle.setFillColor(sf::Color(pow(_output[i], 2) * 255, pow(_output[i], 2) * 255, pow(_output[i], 2) * 255));
            rectangle.setPosition({i * 1.0f, windowSize[1] - _output[i] * windowSize[1] * 1.0f});
            window.draw(rectangle);
        }

        if (delayClock.getElapsedTime().asSeconds() >= 0.3)
        {
            FPS.setString("FPS: " + std::to_string((int)(1 / fpsClock.getElapsedTime().asSeconds())));
            delayClock.restart();
            // time(&FPSCounter);
            // FOVtext.setString("Rotation: " + std::to_string(rotationXOffset));
        }
        window.draw(FPS);
        window.draw(FOVtext);

        window.display();
        // clock.restart();
    }
}
