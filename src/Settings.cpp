// SPDX-FileCopyrightText: 2024 SemkiShow
//
// SPDX-License-Identifier: MIT

#include "Render.hpp"
#include "Settings.hpp"
#include "UI.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int scale = 10;
int fov = 90;
int renderDistance = 20;
std::string mapID = "map0";
Color wallColor = {255, 255, 255, 255};
bool vsync = true;
int rotationSensitivity = 200;
int movementSensitivity = 2;
bool showFPS = true;

std::vector<std::string> Split(const std::string& input, char delimiter = ' ')
{
    std::vector<std::string> output;
    output.push_back("");
    int index = 0;
    for (size_t i = 0; i < input.size(); i++)
    {
        if (input[i] == delimiter)
        {
            output.push_back("");
            index++;
            continue;
        }
        output[index] += input[i];
    }
    return output;
}

std::string TrimJunk(const std::string& input)
{
    auto first = input.find_first_not_of("\t\n\r\f\v");
    auto last = input.find_last_not_of("\t\n\r\f\v");
    return (first == input.npos) ? "" : input.substr(first, last - first + 1);
}

void LoadMap()
{
    std::ifstream mapFile("resources/maps/" + mapID + ".txt");
    std::string buf;
    std::vector<std::string> scaleOneMap;
    while (std::getline(mapFile, buf))
        scaleOneMap.push_back(buf);
    mapFile.close();

    map.clear();
    for (size_t i = 0; i < scaleOneMap.size(); i++)
    {
        for (int j = 0; j < scale; j++)
        {
            map.push_back("");
            for (size_t k = 0; k < scaleOneMap[0].size(); k++)
            {
                map[map.size() - 1] += std::string(scale, scaleOneMap[i][k]);
            }
        }
    }

    GetPlayerPosition();
}

void Save()
{
    // Read the file
    std::ofstream file("settings.txt");
    file << "scale=" << scale << '\n';
    file << "fov=" << fov << '\n';
    file << "render-distance=" << renderDistance << '\n';
    file << "map-id=" << mapID << '\n';
    file << "wall-color=" << (int)wallColor.r << ',' << (int)wallColor.g << ',' << (int)wallColor.b
         << '\n';
    file << "vsync=" << (vsync ? "true" : "false") << '\n';
    file << "rotation-sensitivity=" << rotationSensitivity << '\n';
    file << "movement-sensitivity=" << movementSensitivity << '\n';
    file << "show-fps=" << (showFPS ? "true" : "false") << '\n';
    file.close();
}

void Load()
{
    std::ifstream file("settings.txt");
    std::string buf, label, value;
    while (std::getline(file, buf))
    {
        auto split = Split(buf, '=');
        if (split.size() < 2)
        {
            std::cout << "Error: invalid settings.txt!\n";
            continue;
        }
        label = TrimJunk(split[0]);
        value = TrimJunk(split[1]);

        if (label == "scale") scale = stoi(value);
        if (label == "fov") fov = stoi(value);
        if (label == "render-distance") renderDistance = stoi(value);
        if (label == "map-id") mapID = value;
        if (label == "wall-color")
        {
            auto color = Split(value, ',');
            wallColor.r = stoi(color[0]);
            wallColor.g = stoi(color[1]);
            wallColor.b = stoi(color[2]);
            wallColor.a = 255;
        }
        if (label == "vsync") vsync = value == "true";
        if (label == "rotation-sensitivity") rotationSensitivity = stod(value);
        if (label == "movement-sensitivity") movementSensitivity = stod(value);
        if (label == "show-fps") showFPS = value == "true";
    }
    file.close();

    if (!std::filesystem::exists("resources/maps/" + mapID + ".txt")) mapID = "map0";
    LoadMap();
    mapID.reserve(1024);
}
