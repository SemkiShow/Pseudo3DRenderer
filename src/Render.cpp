#include "Render.h"

// Public variables init
int FOV = 90;
double rotationXOffset = 0;
double sensitivity = 200;
int renderDistance = 100;

double DegreesToRadians(double _degrees)
{
    return _degrees * (3.141592654 / 180);
}

int* AngleToRayDestination(double _angle, int* _playerPosition)
{
    int* _rayDestination = new int[2];
    _rayDestination[0] = _playerPosition[0] + (int)(cos(DegreesToRadians(_angle)) * renderDistance);
    _rayDestination[1] = _playerPosition[1] + (int)(sin(DegreesToRadians(_angle)) * renderDistance);
    return _rayDestination;
}

int* RayCollisionDetection(int* _rayDestination, std::string* _map, int _mapSize, int* _playerPosition)
{
    double* _rayStep = new double[2];
    _rayStep[0] = (_rayDestination[0] - _playerPosition[0]) * 1.0 / renderDistance;
    _rayStep[1] = (_rayDestination[1] - _playerPosition[1]) * 1.0 / renderDistance;
    
    int* _rayPosition = new int[3];
    _rayPosition[0] = 0;
    _rayPosition[1] = 0;
    _rayPosition[3] = 0; // Ray distance
    for (int i = 0; i < renderDistance; i++)
    {
        _rayPosition[0] = _playerPosition[0] + (int)(_rayStep[0] * i);
        _rayPosition[1] = _playerPosition[1] + (int)(_rayStep[1] * i);
        // std::cout << _map[_rayPosition[1]][_rayPosition[0]] << "; ";
        if (_rayPosition[0] < 0 | _rayPosition[0] >= _map[0].length() | _rayPosition[1] < 0 | _rayPosition[1] >= _mapSize)
        {
            _rayPosition[3] = renderDistance;
            break;
        }
        else
        {
            if (_map[_rayPosition[1]][_rayPosition[0]] == '#')
            {
                break;
            }
        }
        _rayPosition[3]++;
    }
    return _rayPosition;
}

int* GetPlayerPosition(std::string* _map, int _mapSize)
{
    static int _playerPosition[2] = {-1, -2};
    std::cout << "The map size is " << _mapSize << ", " << _map[0].length() << std::endl;
    
    for (int y = 0; y < _mapSize; y++)
    {
        for (int x = 0; x < _map[y].length(); x++)
        {
            std::cout << _map[y][x];
            if (_map[y][x] == 'P')
            {
                _playerPosition[0] = x;
                _playerPosition[1] = y;
                // break;
            }
            
        }
        std::cout << std::endl;
    }
    return _playerPosition;
}

double* RenderFrame(std::string* _map, int _mapSize, int* _windowSize, int* _playerPosition)
{
    double* _output = new double[_windowSize[0]];
    for (int i = 0; i < _windowSize[0]; i++)
    {
        _output[i] = 0.5;
    }
    double _rayAngleStep = FOV * 1.0 / _windowSize[0];
    int* _rayPosition;
    // std::cout << (int)(_rayPosition[3] * 1.0 / renderDistance * _windowSize[1]);
    
    // std::cout << "Ray angle, Ray distance, Column height, Symbol index, Symbol" << std::endl;
    for (int i = 0; i < _windowSize[0]; i++)
    {
        _rayPosition = RayCollisionDetection(AngleToRayDestination(_rayAngleStep * i + rotationXOffset, _playerPosition), _map, _mapSize, _playerPosition);
        // std::cout << (int)(_rayAngleStep * i) << " deg, " << _rayPosition[3] << ", " << (int)(_rayPosition[3] * 1.0 / renderDistance * _windowSize[1]) << ", " << (int)((1.0 - _rayPosition[3] * 1.0 / renderDistance) * (brightness.length() - 1)) << ", " << brightness[(int)((1.0 - _rayPosition[3] * 1.0 / renderDistance) * (brightness.length() - 1))] << "; ";
        _output[i] = 1 - _rayPosition[3] * 1.0 / renderDistance;
        // std::cout << std::endl;
    }
    // std::cout << std::endl;

    return _output;
}
