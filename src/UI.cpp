// SPDX-FileCopyrightText: 2024 SemkiShow
//
// SPDX-License-Identifier: MIT

#include "Game.hpp"
#include "Settings.hpp"
#include "UI.hpp"
#include <raygui.h>

#define UI_SPACING 30
#define ELEMENT_SIZE 30
#define SLIDER_WIDTH (float)GetScreenWidth() - 270
#define COLOR_PICKER_SIZE 100
#define ELEMENT_SPACING 10

bool isSettings = false;

float nextElementPositionY = UI_SPACING * 2;

void DrawCheckBox(const char* text, bool* value)
{
    GuiCheckBox(Rectangle{UI_SPACING * 2, nextElementPositionY, ELEMENT_SIZE, ELEMENT_SIZE}, text,
                value);
    nextElementPositionY += ELEMENT_SIZE + ELEMENT_SPACING;
}

void DrawSlider(const char* leftText, const char* rightText, float* value, float minValue,
                float maxValue)
{
    GuiSlider(Rectangle{UI_SPACING * 2, nextElementPositionY, SLIDER_WIDTH, ELEMENT_SIZE}, leftText,
              rightText, value, minValue, maxValue);
    DrawText(std::to_string(*value).c_str(), (SLIDER_WIDTH + UI_SPACING * 2) / 2.f,
             nextElementPositionY + 5, 24, WHITE);
    nextElementPositionY += ELEMENT_SIZE + ELEMENT_SPACING;
}

void DrawSliderInt(const char* leftText, const char* rightText, int* value, float minValue,
                   float maxValue)
{
    float valueFloat = *value;
    GuiSlider(Rectangle{UI_SPACING * 2, nextElementPositionY, SLIDER_WIDTH, ELEMENT_SIZE}, leftText,
              rightText, &valueFloat, minValue, maxValue);
    *value = valueFloat;
    DrawText(std::to_string(*value).c_str(), (SLIDER_WIDTH + UI_SPACING * 2) / 2.f,
             nextElementPositionY + 5, 24, WHITE);
    nextElementPositionY += ELEMENT_SIZE + ELEMENT_SPACING;
}

void DrawColorPicker(const char* text, Color* color)
{
    GuiColorPanel(
        Rectangle{UI_SPACING * 2, nextElementPositionY, COLOR_PICKER_SIZE, COLOR_PICKER_SIZE}, text,
        color);
    nextElementPositionY += COLOR_PICKER_SIZE + ELEMENT_SPACING;
}

void DrawTextBox(const char* label, std::string& text)
{
    GuiTextBox(Rectangle{UI_SPACING * 2, nextElementPositionY, SLIDER_WIDTH, ELEMENT_SIZE},
               text.data(), text.capacity(), true);
    DrawText(label, SLIDER_WIDTH + UI_SPACING * 2 + 5, nextElementPositionY + 5, 24, WHITE);
    nextElementPositionY += ELEMENT_SIZE + ELEMENT_SPACING;
}

void DrawSettings()
{
    if (!isSettings) return;
    DrawRectangleRounded(Rectangle{UI_SPACING, UI_SPACING, (float)GetScreenWidth() - UI_SPACING * 2,
                                   (float)GetScreenHeight() - UI_SPACING * 2},
                         0.1f, 1, Color{128, 128, 128, 128});
    nextElementPositionY = UI_SPACING * 2;
    DrawSliderInt("", "scale", &scale, 1, 100);
    DrawSliderInt("", "fov", &fov, 10, 180);
    DrawSliderInt("", "render-distance", &renderDistance, 10, 50);
    DrawTextBox("map-id", mapID);
    DrawColorPicker("wall-color", &wallColor);
    DrawCheckBox("vsync", &vsync);
    DrawSliderInt("", "rotation-sensitivity", &rotationSensitivity, 10, 500);
    DrawSliderInt("", "movement-sensitivity", &movementSensitivity, 1, 50);
    DrawCheckBox("show-fps", &showFPS);
}

void DrawUI()
{
    if (GuiButton(Rectangle{(float)GetScreenWidth() - 30, 0, 30, 30}, "#142#"))
        isSettings = !isSettings;
    if (GuiButton(Rectangle{(float)GetScreenWidth() - 60, 0, 30, 30}, "#77#")) LoadMap();

    DrawSettings();
}
