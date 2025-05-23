#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <thread>

extern int FOV;
extern double rotationXOffset;
extern int rotationSensitivity;
extern int movementSensitivity;
extern int scale;
extern int renderDistance;
extern int windowSize[2];
extern double playerPosition[2];
extern std::vector<std::string> map;
extern double* output;
extern int threadsNumber;

double DegreesToRadians(double degrees);
int Raycast(double angle);
void GetPlayerPosition();
void RenderFrame(int threadID);
