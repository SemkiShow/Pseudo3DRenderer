#include "Game.hpp"
#include "UI.hpp"
#include "Render.hpp"

void LoadMap()
{
    std::fstream mapFile;
    mapFile.open("maps/" + mapID + ".txt", std::ios::in);
    std::string buf;
    std::vector<std::string> scaleOneMap;
    while (std::getline(mapFile, buf))
    scaleOneMap.push_back(buf);
    mapFile.close();

    map.clear();
    int counter = 0;
    for (int i = 0; i < scaleOneMap.size(); i++)
    {
        for (int j = 0; j < scale; j++)
        {
            map.push_back("");
            for (int k = 0; k < scaleOneMap[0].size(); k++)
            {
                map[map.size()-1] += std::string(scale, scaleOneMap[i][k]);
            }            
        }
    }

    GetPlayerPosition();
}

void ProcessPlayerInput(sf::Time deltaTime)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        rotationXOffset -= rotationSensitivity * deltaTime.asSeconds();
        if (rotationXOffset < -360) rotationXOffset = 0;
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        rotationXOffset += rotationSensitivity * deltaTime.asSeconds();
        if (rotationXOffset >= 360) rotationXOffset = 0;
    }

    std::string direction = "null";
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
    {
        playerPosition[0] += cos(DegreesToRadians(rotationXOffset + FOV/2)) * movementSensitivity * scale * deltaTime.asSeconds();
        playerPosition[1] += sin(DegreesToRadians(rotationXOffset)) * movementSensitivity * scale * deltaTime.asSeconds();
        direction = "forward";
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
    {
        playerPosition[0] -= cos(DegreesToRadians(rotationXOffset + FOV/2)) * movementSensitivity * scale * deltaTime.asSeconds();
        playerPosition[1] -= sin(DegreesToRadians(rotationXOffset)) * movementSensitivity * scale * deltaTime.asSeconds();
        direction = "backward";
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        playerPosition[0] += cos(DegreesToRadians(rotationXOffset + FOV/2 - 90)) * movementSensitivity * scale * deltaTime.asSeconds();
        playerPosition[1] += sin(DegreesToRadians(rotationXOffset)) * movementSensitivity * scale * deltaTime.asSeconds();
        direction = "left";
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        playerPosition[0] -= cos(DegreesToRadians(rotationXOffset + FOV/2 - 90)) * movementSensitivity * scale * deltaTime.asSeconds();
        playerPosition[1] -= sin(DegreesToRadians(rotationXOffset)) * movementSensitivity * scale * deltaTime.asSeconds();
        direction = "right";
    }

    for (int i = 0; i < 2; i++)
    {
        if (playerPosition[i] < 0) playerPosition[i] = 0;
        if (playerPosition[i] >= map[0].size() && i == 0) playerPosition[i] = map[0].size() - 1;
        if (playerPosition[i] >= map.size() && i == 1) playerPosition[i] = map.size() - 1;
    }
    if (map[(int)playerPosition[1]][(int)playerPosition[0]] == '#')
    {
        playerPosition[0] += cos(DegreesToRadians(rotationXOffset + FOV/2 - (direction == "left" || direction == "right" ? 90 : 0))) * 
            movementSensitivity * scale * deltaTime.asSeconds() * 2 * (direction == "forward" || direction == "right" ? -1 : 1);
        playerPosition[1] += sin(DegreesToRadians(rotationXOffset)) * movementSensitivity * scale * deltaTime.asSeconds() * 2 * (direction == "forward" ? -1 : 1);
    }
}
