#include "imgui.h"
#include "imgui-SFML.h"

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
std::string mapID = "0";
double* playerPosition;
std::string* _map;
int _mapSize;

// GUI
int menuOffset = 20;
int scaleValue = 10;
int fovValue = 90;
int renderDistanceValue = 200;
bool isSettings = false;
int lastScale = scale;
size_t mapIDSize = 255;
std::string lastMapID = mapID;

class Settings
{
    public:
        void Save(std::string fileName);
        void Load(std::string fileName);
        float wallColor[3] = {1, 1, 1};
}settings;

std::string* Split(std::string input, char delimiter = ' ')
{
    std::vector<std::string> output;
    output.push_back("");
    int index = 0;
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] == delimiter)
        {
            index++;
            output.push_back("");
            continue;
        }
        output[index] += input[i];
    }
    std::string* outputArray = new std::string[output.size()];
    std::copy(output.begin(), output.end(), outputArray);
    return outputArray;
}

void Settings::Save(std::string fileName)
{
    // Read the file
    std::fstream settingsFile;
    settingsFile.open(fileName, std::ios::out);
    settingsFile << "scale=" << scale << '\n';
    settingsFile << "fov=" << FOV << '\n';
    settingsFile << "render-distance=" << renderDistance << '\n';
    settingsFile << "map-id=" << mapID << '\n';
    settingsFile << "wall-color=" << wallColor[0] << ',' << wallColor[1] << ',' << wallColor[2] << '\n';
    settingsFile.close();
}

void Settings::Load(std::string fileName)
{
    // Read the file
    std::fstream settingsFile;
    settingsFile.open(fileName, std::ios::in);
    std::vector<std::string> settingsList;
    std::string buf;
    while (std::getline(settingsFile, buf))
        settingsList.push_back(buf);
    settingsFile.close();

    // Process the file
    scale = stoi(settingsList[0].substr(6));
    FOV = stoi(settingsList[1].substr(4));
    renderDistance = stoi(settingsList[2].substr(16));
    mapID = settingsList[3].substr(7);
    std::string* wallColorArray = Split(settingsList[4].substr(11), ',');
    for (int i = 0; i < 3; i++) wallColor[i] = stof(wallColorArray[i]);
}

void ShowSettings(bool* isOpen)
{
    if (!ImGui::Begin("Settings", isOpen))
    {
        ImGui::End();
        return;
    }
    ImGui::SliderInt("scale", &scale, 1, 100);
    ImGui::SliderInt("fov", &FOV, 10, 180);
    ImGui::SliderInt("render-distance", &renderDistance, 10, 1000);
    ImGui::InputText("map-id", mapID.data(), mapIDSize);
    ImGui::ColorEdit3("wall-color", settings.wallColor);
    ImGui::End();
}

void LoadMap()
{
    std::fstream mapFile("maps/map" + mapID + ".txt");
    std::string mapFileContent;
    std::vector<std::string> _mapVector;
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
    // free(_map);
    _map = new std::string[_mapSize];
    for (int i = 0; i < _mapSize; i++)
    {
        _map[i] = _mapVector[i];
    }

    playerPosition = GetPlayerPosition(_map, _mapSize);
}

void ShowMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Menu"))
        {
            if (ImGui::MenuItem("Reload")) LoadMap();
            if (ImGui::MenuItem("Settings"))
            {
                isSettings = true;
                ShowSettings(&isSettings);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    return;
}

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
    settings.Load("settings.txt");
    // std::cout << "Enter the map ID" << std::endl;
    // std::getline(std::cin, mapID);
    LoadMap();

    // PrintDebugInfo();

    // SFML init
    sf::RenderWindow window(sf::VideoMode({(unsigned int) windowSize[0], (unsigned int) windowSize[1]}), "Pseudo3DRenderer");
    
    // ImGUI init
    (void) ImGui::SFML::Init(window);

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
    FPS.setPosition({0, menuOffset * 1.f});
    FPS.setString(std::to_string(123));
    FPS.setFillColor(sf::Color(0, 255, 255));
    FPS.setCharacterSize(24);
    sf::Text FOVtext(font);
    FOVtext.setPosition({0, 25});
    FOVtext.setCharacterSize(24);

    // time_t FPSCounter;
    // time(&FPSCounter);
    double* _output;
    sf::Clock deltaTimeClock;
    sf::Time deltaTime;
    sf::Clock delayClock;

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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            rotationXOffset -= rotationSensitivity * deltaTime.asSeconds();
            if (rotationXOffset >= 360) rotationXOffset = 0;
            if (rotationXOffset <= -360) rotationXOffset = 0;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            rotationXOffset += rotationSensitivity * deltaTime.asSeconds();
            if (rotationXOffset >= 360) rotationXOffset = 0;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        {
            playerPosition[0] += cos(DegreesToRadians(rotationXOffset + FOV/2)) * movementSensitivity * deltaTime.asSeconds();
            playerPosition[1] += sin(DegreesToRadians(rotationXOffset)) * movementSensitivity * deltaTime.asSeconds();
            if (_map[(int)playerPosition[1]][(int)playerPosition[0]] == '#')
            {
                playerPosition[0] -= cos(DegreesToRadians(rotationXOffset + FOV/2)) * movementSensitivity * deltaTime.asSeconds() * 2;
                playerPosition[1] -= sin(DegreesToRadians(rotationXOffset)) * movementSensitivity * deltaTime.asSeconds() * 2;
            }
            if (rotationXOffset >= 360) rotationXOffset = 0;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        {
            playerPosition[0] -= cos(DegreesToRadians(rotationXOffset + FOV/2)) * movementSensitivity * deltaTime.asSeconds();
            playerPosition[1] -= sin(DegreesToRadians(rotationXOffset)) * movementSensitivity * deltaTime.asSeconds();
            if (_map[(int)playerPosition[1]][(int)playerPosition[0]] == '#')
            {
                playerPosition[0] += cos(DegreesToRadians(rotationXOffset + FOV/2)) * movementSensitivity * deltaTime.asSeconds() * 2;
                playerPosition[1] += sin(DegreesToRadians(rotationXOffset)) * movementSensitivity * deltaTime.asSeconds() * 2;
            }
            if (rotationXOffset >= 360) rotationXOffset = 0;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            playerPosition[0] += cos(DegreesToRadians(rotationXOffset + FOV/2 - 90)) * movementSensitivity * deltaTime.asSeconds();
            playerPosition[1] += sin(DegreesToRadians(rotationXOffset)) * movementSensitivity * deltaTime.asSeconds();
            if (_map[(int)playerPosition[1]][(int)playerPosition[0]] == '#')
            {
                playerPosition[0] -= cos(DegreesToRadians(rotationXOffset + FOV/2 - 90)) * movementSensitivity * deltaTime.asSeconds() * 2;
                playerPosition[1] -= sin(DegreesToRadians(rotationXOffset)) * movementSensitivity * deltaTime.asSeconds() * 2;
            }
            if (rotationXOffset >= 360) rotationXOffset = 0;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            playerPosition[0] -= cos(DegreesToRadians(rotationXOffset + FOV/2 - 90)) * movementSensitivity * deltaTime.asSeconds();
            playerPosition[1] -= sin(DegreesToRadians(rotationXOffset)) * movementSensitivity * deltaTime.asSeconds();
            if (_map[(int)playerPosition[1]][(int)playerPosition[0]] == '#')
            {
                playerPosition[0] += cos(DegreesToRadians(rotationXOffset + FOV/2 - 90)) * movementSensitivity * deltaTime.asSeconds() * 2;
                playerPosition[1] += sin(DegreesToRadians(rotationXOffset)) * movementSensitivity * deltaTime.asSeconds() * 2;
            }
            if (rotationXOffset >= 360) rotationXOffset = 0;
        }

        window.clear();

        _output = RenderFrame(_map, _mapSize, windowSize, playerPosition);
        for (int i = 0; i < windowSize[0]; i++)
        {
            double wallSize = _output[i] * windowSize[1] - (windowSize[1] - _output[i] * windowSize[1]);
            rectangle.setSize(sf::Vector2f(1, wallSize > 0 ? wallSize : 0));
            rectangle.setFillColor(sf::Color(_output[i] * settings.wallColor[0] * 255, _output[i] * settings.wallColor[1] * 255, _output[i] * settings.wallColor[2] * 255));
            rectangle.setPosition({i * 1.0f, (float)(windowSize[1] - _output[i] * windowSize[1])});
            window.draw(rectangle);
        }

        if (delayClock.getElapsedTime().asSeconds() >= 0.3)
        {
            FPS.setString("FPS: " + std::to_string((int)(1 / deltaTime.asSeconds())));
            delayClock.restart();
            
            movementSensitivity = 2 * scale;
            if (lastScale != scale || lastMapID != mapID)
            {
                lastScale = scale;
                lastMapID = mapID;
                LoadMap();
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
