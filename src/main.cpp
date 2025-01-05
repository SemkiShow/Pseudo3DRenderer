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

    std::cout << "Enter the map ID" << std::endl;
    std::string mapID;
    std::getline(std::cin, mapID);
    std::fstream mapFile("./maps/map" + mapID + ".txt");
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

    PrintDebugInfo();

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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            rotationXOffset -= rotationSensitivity * deltaTime;
            if (rotationXOffset >= 360) rotationXOffset = 0;
            if (rotationXOffset <= -360) rotationXOffset = 0;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            rotationXOffset += rotationSensitivity * deltaTime;
            if (rotationXOffset >= 360) rotationXOffset = 0;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            playerPosition[0] += cos(DegreesToRadians(rotationXOffset)) * movementSensitivity * deltaTime;
            playerPosition[1] += sin(DegreesToRadians(rotationXOffset)) * movementSensitivity * deltaTime;
            if (_map[(int)playerPosition[1]][(int)playerPosition[0]] == '#')
            {
                playerPosition[0] -= cos(DegreesToRadians(rotationXOffset)) * movementSensitivity * deltaTime * 2;
                playerPosition[1] -= sin(DegreesToRadians(rotationXOffset)) * movementSensitivity * deltaTime * 2;
            }
            if (rotationXOffset >= 360) rotationXOffset = 0;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            playerPosition[0] -= cos(DegreesToRadians(rotationXOffset)) * movementSensitivity * deltaTime;
            playerPosition[1] -= sin(DegreesToRadians(rotationXOffset)) * movementSensitivity * deltaTime;
            if (_map[(int)playerPosition[1]][(int)playerPosition[0]] == '#')
            {
                playerPosition[0] += cos(DegreesToRadians(rotationXOffset)) * movementSensitivity * deltaTime * 2;
                playerPosition[1] += sin(DegreesToRadians(rotationXOffset)) * movementSensitivity * deltaTime * 2;
            }
            if (rotationXOffset >= 360) rotationXOffset = 0;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            playerPosition[0] -= cos(DegreesToRadians(rotationXOffset - 45)) * movementSensitivity * deltaTime;
            playerPosition[1] -= sin(DegreesToRadians(rotationXOffset + 90)) * movementSensitivity * deltaTime;
            if (_map[(int)playerPosition[1]][(int)playerPosition[0]] == '#')
            {
                playerPosition[0] += cos(DegreesToRadians(rotationXOffset - 45)) * movementSensitivity * deltaTime * 2;
                playerPosition[1] += sin(DegreesToRadians(rotationXOffset + 90)) * movementSensitivity * deltaTime * 2;
            }
            if (rotationXOffset >= 360) rotationXOffset = 0;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            playerPosition[0] += cos(DegreesToRadians(rotationXOffset - 45)) * movementSensitivity * deltaTime;
            playerPosition[1] += sin(DegreesToRadians(rotationXOffset + 90)) * movementSensitivity * deltaTime;
            if (_map[(int)playerPosition[1]][(int)playerPosition[0]] == '#')
            {
                playerPosition[0] -= cos(DegreesToRadians(rotationXOffset - 45)) * movementSensitivity * deltaTime * 2;
                playerPosition[1] -= sin(DegreesToRadians(rotationXOffset + 90)) * movementSensitivity * deltaTime * 2;
            }
            if (rotationXOffset >= 360) rotationXOffset = 0;
        }


        window.clear();

        _output = RenderFrame(_map, _mapSize, windowSize, playerPosition);
        for (int i = 0; i < windowSize[0]; i++)
        {
            double wallSize = _output[i] * windowSize[1] - (windowSize[1] - _output[i] * windowSize[1]);
            rectangle.setSize(sf::Vector2f(1, wallSize > 0 ? wallSize : 0));
            rectangle.setFillColor(sf::Color(pow(_output[i], 3) * 255, pow(_output[i], 3) * 255, pow(_output[i], 3) * 255));
            rectangle.setPosition(i, windowSize[1] - _output[i] * windowSize[1]);
            window.draw(rectangle);
        }

        if (time(0) - FPSCounter >= 1)
        {
            FPS.setString("FPS: " + std::to_string((int)(1 / clock.getElapsedTime().asSeconds())));
            time(&FPSCounter);
            // FOVtext.setString("Rotation: " + std::to_string(rotationXOffset));
        }
        window.draw(FPS);
        window.draw(FOVtext);

        window.display();
        // clock.restart();
    }
}
