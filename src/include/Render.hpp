#include <iostream>
#include <cmath>
#include <string>
#include <vector>

extern int FOV;
extern double rotationXOffset;
extern int rotationSensitivity;
extern int movementSensitivity;
extern int scale;
extern int renderDistance;
extern int windowSize[2];
extern double playerPosition[2];
extern std::vector<std::string> map;

double DegreesToRadians(double degrees);
int* AngleToRayDestination(double angle);
int* RayCollisionDetection(int* rayDestination);
void GetPlayerPosition();
double* RenderFrame();
