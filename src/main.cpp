#include <SFML/Graphics.hpp>
#include "imgui-SFML.h"
#include <vector>
#include <fstream>
#include "Render.hpp"
#include "UI.hpp"
#include "Settings.hpp"
#include "Game.hpp"

int PrintDebugInfo()
{
    // Debug info
    std::cout << "<Debug info>" << std::endl;

    std::cout << "Window size is " << windowSize[0] << ", " << windowSize[1] << std::endl;

    // std::cout << map[0].size() << std::endl;
    std::cout << "Player position: " << playerPosition[0] << ", " << playerPosition[1] << std::endl;
    
    int angle;
    bool _askForAngle = false;
    if (_askForAngle)
    {
        std::cout << "Enter angle: ";
        std::cin >> angle;
    }
    else
    {
        angle = 42.0; // Veritasium angle
    }
    int* rayDestination = AngleToRayDestination(angle);
    std::cout << "Ray destination at angle " << angle << " is " << rayDestination[0] << ", " << rayDestination[1] << std::endl;

    int* rayPosition = RayCollisionDetection(rayDestination);
    std::cout << "Ray position is " << rayPosition[0] << ", " << rayPosition[1] << ". The ray has travelled " << rayPosition[3] << " m" << std::endl;

    double* output = RenderFrame();

    std::cout << "</Debug info>" << std::endl;

    return 0;
}

int main()
{
    settings.Load("settings.txt");

    if (!std::filesystem::exists("maps/" + mapID + ".txt")) mapID = "map0";
    LoadMap();

    // PrintDebugInfo();

    // SFML init
    sf::RenderWindow window(sf::VideoMode({(unsigned int)windowSize[0], (unsigned int)windowSize[1]}), "Pseudo3DRenderer");
    window.setFramerateLimit(0);
    
    // ImGUI init
    (void) ImGui::SFML::Init(window);

    if (settings.verticalSync)
        window.setVerticalSyncEnabled(true);
    else
        window.setVerticalSyncEnabled(false);

    sf::RectangleShape rectangle(sf::Vector2f(0, 0));
    // rectangle.setFillColor(Color(127, 127, 255));
    rectangle.setFillColor(sf::Color(0, 132, 208));
    // rectangle.setFillColor(Color(34, 165, 241));

    sf::Font font("assets/JetBrainsMonoNerdFont-Medium.ttf");
    sf::Text FPS(font);
    FPS.setPosition({0, menuOffset * 1.f});
    FPS.setString(std::to_string(123));
    FPS.setFillColor(sf::Color(0, 255, 255));
    FPS.setCharacterSize(24);
    sf::Text FOVtext(font);
    FOVtext.setPosition({0, 25});
    FOVtext.setCharacterSize(24);

    double* output;
    sf::Clock deltaTimeClock;
    sf::Time deltaTime;
    sf::Clock delayClock;
    bool lastVSync = settings.verticalSync;
    int lastScale = scale;
    std::string lastMapID = mapID;

    // Main loop
    while (window.isOpen())
    {
        deltaTime = deltaTimeClock.restart();
        ImGui::SFML::Update(window, deltaTime);

        while (const auto event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        ShowMenuBar();
        if (isSettings) ShowSettings(&isSettings);

        ProcessPlayerInput(deltaTime);

        window.clear();

        output = RenderFrame();
        for (int i = 0; i < windowSize[0]; i++)
        {
            double wallSize = output[i] * windowSize[1] - (windowSize[1] - output[i] * windowSize[1]);
            rectangle.setSize(sf::Vector2f(1, wallSize > 0 ? wallSize : 0));
            rectangle.setFillColor(sf::Color(output[i] * settings.wallColor[0] * 255, output[i] * settings.wallColor[1] * 255, output[i] * settings.wallColor[2] * 255));
            rectangle.setPosition({i * 1.f, (float)(windowSize[1] - output[i] * windowSize[1])});
            window.draw(rectangle);
        }
        delete output;

        if (delayClock.getElapsedTime().asSeconds() >= 0.3)
        {
            delayClock.restart();
            FPS.setString("FPS: " + std::to_string((int)(1 / deltaTime.asSeconds())));
            
            if (lastScale != scale || lastMapID != mapID)
            {
                lastScale = scale;
                lastMapID = mapID;
                if (std::filesystem::exists("maps/" + mapID + ".txt")) LoadMap();
            }
            if (lastVSync != settings.verticalSync)
            {
                lastVSync = settings.verticalSync;
                if (settings.verticalSync)
                    window.setVerticalSyncEnabled(true);
                else
                    window.setVerticalSyncEnabled(false);
            }
        }
        window.draw(FPS);
        window.draw(FOVtext);

        ImGui::SFML::Render(window);
        window.display();
    }

    settings.Save("settings.txt");
    ImGui::SFML::Shutdown();
}
