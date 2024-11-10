// #include <iostream>
// #include <cmath>
// #include <random>
// #include <vector>
// using namespace std;
#include <iostream>
#include <cmath>
// #include <vector>
using namespace std;

float DegreesToRadians(float _degrees);
int* AngleToRayDestination(float _angle, int* _playerPosition);
int* RayCollisionDetection(int* _rayDestination, string* _map, int _mapSize, int* _playerPosition);
int* GetPlayerPosition(string* _map, int _mapSize);
float* RenderFrame(string* _map, int _mapSize, int* _windowSize, int* _playerPosition);
