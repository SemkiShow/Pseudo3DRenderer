#include "UI.hpp"
#include "Settings.hpp"
#include "Render.hpp"
#include "Game.hpp"

std::string mapID = "0";

bool isSettings = false;
int menuOffset = 20;

void ShowSettings(bool* isOpen)
{
    if (!ImGui::Begin("Settings", isOpen))
    {
        ImGui::End();
        return;
    }
    ImGui::SliderInt("scale", &scale, 1, 100);
    ImGui::SliderInt("fov", &FOV, 10, 180);
    ImGui::SliderInt("render-distance", &renderDistance, 10, 50);
    ImGui::InputText("map-id", &mapID, ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::ColorEdit3("wall-color", settings.wallColor);
    ImGui::Checkbox("vsync", &settings.verticalSync);
    ImGui::SliderInt("rotation-sensitivity", &rotationSensitivity, 10, 500);
    ImGui::SliderInt("movement-sensitivity", &movementSensitivity, 1, 50);
    ImGui::End();
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
