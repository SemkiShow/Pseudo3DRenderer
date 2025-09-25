#pragma once

#include <raylib.h>
#include <string>

extern int scale;
extern int fov;
extern int renderDistance;
extern std::string mapID;
extern Color wallColor;
extern bool vsync;
extern int rotationSensitivity;
extern int movementSensitivity;
extern bool showFPS;

void LoadMap();
void Save();
void Load();
