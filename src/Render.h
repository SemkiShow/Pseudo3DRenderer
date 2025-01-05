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

double DegreesToRadians(double _degrees);
int* AngleToRayDestination(double _angle, int* _playerPosition);
int* RayCollisionDetection(int* _rayDestination, std::string* _map, int _mapSize, int* _playerPosition);
int* GetPlayerPosition(std::string* _map, int _mapSize);
double* RenderFrame(std::string* _map, int _mapSize, int* _windowSize, int* _playerPosition);
