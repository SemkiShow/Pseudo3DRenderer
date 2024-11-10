#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include "Render.h"
using namespace sf;
using namespace std;

// Public variables init
// string brightness = " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";
// string brightness = "#";
int* playerPosition;
int windowSize[2] = {16*32*2, 9*32*2};
int verticalSync = true;

int main()
{
    // Local variables init
    RenderWindow window(VideoMode(windowSize[0], windowSize[1]), "Pseudo3DShooter");
    glEnable(GL_TEXTURE_2D);
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

    RectangleShape rectangle(Vector2f(0, 0));
    // rectangle.setFillColor(Color(127, 127, 255));
    rectangle.setFillColor(Color(0, 132, 208));
    // rectangle.setFillColor(Color(34, 165, 241));

    Font font;
    font.loadFromFile("../JetBrainsMonoNerdFont-Medium.ttf");
    Text FPS;
    FPS.setFont(font);
    FPS.setString(to_string(123));
    FPS.setCharacterSize(24);

    vector<string> _mapVector;
    string mapFileName = "map2";
    fstream mapFile("../maps/" + mapFileName + ".txt");
    string mapFileContent;
    while (getline(mapFile, mapFileContent))
    {
        // cout << mapFileContent << endl;
        _mapVector.push_back(mapFileContent);
    }
    mapFile.close();
    int _mapSize = _mapVector.size();
    string* _map = new string[_mapSize];
    for (int i = 0; i < _mapSize; i++)
    {
        _map[i] = _mapVector[i];
    }
    
    // cout << mapFileContent << endl;

    // string* _map = new string[sizeof(map2) / sizeof(*map2)];
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
    cout << "<Debug info>" << endl;

    // cout << brightness << endl;
    // cout << 3.141592654 << endl;  

    // GetTerminalSize(windowSize[0], windowSize[1]);
    cout << "Window size is " << windowSize[0] << ", " << windowSize[1] << endl;

    playerPosition = GetPlayerPosition(_map, _mapSize);
    // cout << _map[0].length() << endl;
    cout << "Player position: " << playerPosition[0] << ", " << playerPosition[1] << endl;
    
    int _angle;
    bool _askForAngle = false;
    if (_askForAngle)
    {
        cout << "Enter angle: ";
        cin >> _angle;
    }
    else
    {
        _angle = 42.0; // Veritasium angle
    }
    int* _rayDestination = AngleToRayDestination(_angle, playerPosition);
    cout << "Ray destination at angle " << _angle << " is " << _rayDestination[0] << ", " << _rayDestination[1] << endl;

    int* _rayPosition = RayCollisionDetection(_rayDestination, _map, _mapSize, playerPosition);
    cout << "Ray position is " << _rayPosition[0] << ", " << _rayPosition[1] << ". The ray has travelled " << _rayPosition[3] << " m" << endl;

    // float* _output = new float[windowSize[0]];
    float* _output = RenderFrame(_map, _mapSize, windowSize, playerPosition);
    for (int i = 0; i < windowSize[0]; i++)
    {
        cout << _output[i] << "; ";
    }
    cout << endl;

    cout << "</Debug info>" << endl;

    time_t FPSCounter;
    time(&FPSCounter);
    Clock clock;

    // Main loop
    while (window.isOpen())
    {
        clock.restart();

        for (auto event = Event{}; window.pollEvent(event);)
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
        }

        window.clear();

        // _output = RenderFrame(_map, _mapSize, windowSize, playerPosition);
        // for (int i = 0; i < windowSize[0]; i++)
        // {
        //     rectangle.setSize(Vector2f(1, _output[i] * windowSize[1]));
        //     rectangle.setPosition(i, windowSize[1] - _output[i] * windowSize[1]);
        //     window.draw(rectangle);
        // }

        if (time(NULL) - FPSCounter >= 1)
        {
            FPS.setString(to_string((int)(1 / clock.getElapsedTime().asSeconds())));
            time(&FPSCounter);
        }
        window.draw(FPS);

        window.display();
        // clock.restart();
    }
}
