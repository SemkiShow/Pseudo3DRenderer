#include "Settings.hpp"
#include "Render.hpp"
#include "UI.hpp"

Settings settings;

std::vector<std::string> Split(std::string input, char delimiter = ' ')
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
    return output;
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
    settingsFile << "vsync=" << (verticalSync ? "true" : "false") << '\n';
    settingsFile << "rotation-sensitivity=" << rotationSensitivity << '\n';
    settingsFile << "movement-sensitivity=" << movementSensitivity << '\n';
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
    std::vector<std::string> wallColorArray = Split(settingsList[4].substr(11), ',');
    for (int i = 0; i < 3; i++) wallColor[i] = stof(wallColorArray[i]);
    verticalSync = settingsList[5].substr(6) == "true" ? true : false;
    rotationSensitivity = stoi(settingsList[6].substr(21));
    movementSensitivity = stoi(settingsList[7].substr(21));
}
