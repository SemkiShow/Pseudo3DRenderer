// #include <iostream>
// #include <cmath>
// #include <random>
// #include <vector>
// using namespace std;
#include <iostream>
#include <cmath>
// #include <vector>
using namespace std;

extern int FOV;

double DegreesToRadians(double _degrees);
int* AngleToRayDestination(double _angle, int* _playerPosition);
int* RayCollisionDetection(int* _rayDestination, string* _map, int _mapSize, int* _playerPosition);
int* GetPlayerPosition(string* _map, int _mapSize);
double* RenderFrame(string* _map, int _mapSize, int* _windowSize, int* _playerPosition);
