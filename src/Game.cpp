#include "Game.hpp"
#include "Render.hpp"
#include "Settings.hpp"
#include "UI.hpp"
#include <cmath>
#include <filesystem>
#include <raylib.h>

double delayTime = 0;

int lastScale = scale;
std::string lastMapID = "";
bool lastVsync = vsync;

void ProcessPlayerInput()
{
    double deltaTime = GetFrameTime();

    if (IsKeyDown(KEY_LEFT))
    {
        rotationXOffset -= rotationSensitivity * deltaTime;
        if (rotationXOffset < -360) rotationXOffset = 0;
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        rotationXOffset += rotationSensitivity * deltaTime;
        if (rotationXOffset >= 360) rotationXOffset = 0;
    }

    std::string direction = "null";
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
    {
        playerPosition.x += cos(DegreesToRadians(rotationXOffset + fov / 2.0)) *
                            movementSensitivity * scale * deltaTime;
        playerPosition.y +=
            sin(DegreesToRadians(rotationXOffset)) * movementSensitivity * scale * deltaTime;
        direction = "forward";
    }

    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
    {
        playerPosition.x -= cos(DegreesToRadians(rotationXOffset + fov / 2.0)) *
                            movementSensitivity * scale * deltaTime;
        playerPosition.y -=
            sin(DegreesToRadians(rotationXOffset)) * movementSensitivity * scale * deltaTime;
        direction = "backward";
    }

    if (IsKeyDown(KEY_A))
    {
        playerPosition.x +=
            cos(DegreesToRadians(rotationXOffset + fov)) * movementSensitivity * scale * deltaTime;
        playerPosition.y +=
            sin(DegreesToRadians(rotationXOffset - fov)) * movementSensitivity * scale * deltaTime;
        direction = "left";
    }

    if (IsKeyDown(KEY_D))
    {
        playerPosition.x -=
            cos(DegreesToRadians(rotationXOffset + fov)) * movementSensitivity * scale * deltaTime;
        playerPosition.y -=
            sin(DegreesToRadians(rotationXOffset - fov)) * movementSensitivity * scale * deltaTime;
        direction = "right";
    }

    playerPosition.x = std::fmax(0, playerPosition.x);
    playerPosition.y = std::fmax(0, playerPosition.y);
    playerPosition.x = std::fmin(map[0].size() - 1, playerPosition.x);
    playerPosition.y = std::fmin(map.size() - 1, playerPosition.y);
    if (map[playerPosition.y][playerPosition.x] == '#')
    {
        playerPosition.x +=
            cos(DegreesToRadians(rotationXOffset + fov / 2.0 +
                                 (direction == "left" || direction == "right" ? fov / 2 : 0))) *
            movementSensitivity * scale * deltaTime * 2 *
            (direction == "forward" || direction == "right" ? -1 : 1);
        playerPosition.x +=
            sin(DegreesToRadians(rotationXOffset -
                                 (direction == "left" || direction == "right" ? fov : 0))) *
            movementSensitivity * scale * deltaTime * 2 *
            (direction == "forward" || direction == "right" ? -1 : 1);
    }
}

void DrawFrame()
{
    BeginDrawing();

    ClearBackground(BLACK);

    ProcessPlayerInput();

    auto frame = GetFrame();
    for (size_t i = 0; i < frame.size(); i++)
    {
        double wallHeight = frame[i] * windowSize.y - (windowSize.y - frame[i] * windowSize.y);
        Color color = wallColor;
        color.r *= frame[i];
        color.g *= frame[i];
        color.b *= frame[i];
        DrawRectangleRec({i * 1.f, float(windowSize.y - frame[i] * windowSize.y), 1,
                          (float)std::fmax(0, wallHeight)},
                         color);
    }

    DrawUI();

    if (showFPS) DrawFPS(0, 0);
    if (GetTime() - delayTime >= 0.3)
    {
        delayTime = GetTime();
        if (lastScale != scale || lastMapID != mapID)
        {
            lastScale = scale;
            lastMapID = mapID;
            if (std::filesystem::exists("resources/maps/" + mapID + ".txt")) LoadMap();
        }
        if (lastVsync != vsync)
        {
            lastVsync = vsync;
            if (vsync)
                SetWindowState(FLAG_VSYNC_HINT);
            else
                ClearWindowState(FLAG_VSYNC_HINT);
        }
    }

    EndDrawing();
}
