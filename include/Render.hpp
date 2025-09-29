// SPDX-FileCopyrightText: 2024 SemkiShow
//
// SPDX-License-Identifier: MIT

#pragma once

#include <raylib.h>
#include <string>
#include <vector>

extern double rotationXOffset;
extern Vector2 windowSize;
extern Vector2 playerPosition;
extern std::vector<std::string> map;

double DegreesToRadians(double degrees);
void GetPlayerPosition();
std::vector<double> GetFrame();
