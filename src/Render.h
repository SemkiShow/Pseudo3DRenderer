// #include <iostream>
// #include <cmath>
// #include <random>
// #include <vector>
// using namespace std;
#include <iostream>
#include <cmath>
#include <string>
// #include <vector>
// using namespace std;

extern int FOV;
extern double rotationXOffset;
extern int rotationSensitivity;
extern double movementSensitivity;
extern int scale;
extern int renderDistance;

double DegreesToRadians(double _degrees);
int* AngleToRayDestination(double _angle, double* _playerPosition);
int* RayCollisionDetection(int* _rayDestination, std::string* _map, int _mapSize, double* _playerPosition);
double* GetPlayerPosition(std::string* _map, int _mapSize);
double* RenderFrame(std::string* _map, int _mapSize, int* _windowSize, double* _playerPosition);
