#include "Render.h"

// Public variables init
int FOV = 90;
int renderDistance = 5;

float DegreesToRadians(float _degrees)
{
    return _degrees * (3.141592654f / 180);
}

int* AngleToRayDestination(float _angle, int* _playerPosition)
{
    int* _rayDestination = new int[2];
    _rayDestination[0] = _playerPosition[0] + (int)(cos(DegreesToRadians(_angle)) * renderDistance);
    _rayDestination[1] = _playerPosition[1] + (int)(sin(DegreesToRadians(_angle)) * renderDistance);
    return _rayDestination;
}

int* RayCollisionDetection(int* _rayDestination, string* _map, int _mapSize, int* _playerPosition)
{
    float* _rayStep = new float[2];
    _rayStep[0] = (_rayDestination[0] - _playerPosition[0]) * 1.0f / renderDistance;
    _rayStep[1] = (_rayDestination[1] - _playerPosition[1]) * 1.0f / renderDistance;
    
    int* _rayPosition = new int[3];
    _rayPosition[0] = 0;
    _rayPosition[1] = 0;
    _rayPosition[3] = 0; // Ray distance
    for (int i = 0; i < renderDistance; i++)
    {
        _rayPosition[0] = _playerPosition[0] + (int)(_rayStep[0] * i);
        _rayPosition[1] = _playerPosition[1] + (int)(_rayStep[1] * i);
        // cout << _map[_rayPosition[1]][_rayPosition[0]] << "; ";
        if (_rayPosition[0] < 0 | _rayPosition[0] > _map[0].length() | _rayPosition[1] < 0 | _rayPosition[1] > _mapSize)
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

int* GetPlayerPosition(string* _map, int _mapSize)
{
    static int _playerPosition[2] = {-1, -2};
    cout << "The map size is " << _mapSize/* (sizeof(_map) / sizeof(_map[0])) */ << ", " << _map[0].length() << endl;
    
    for (int y = 0; y < _mapSize; y++)
    {
        for (int x = 0; x < _map[y].length(); x++)
        {
            cout << _map[y][x];
            if (_map[y][x] == 'P')
            {
                _playerPosition[0] = x;
                _playerPosition[1] = y;
                // break;
            }
            
        }
        /* if (_playerPosition[0] != -1)
        {
            break;
        } */
        cout << endl;
    }
    return _playerPosition;
}

float* RenderFrame(string* _map, int _mapSize, int* _windowSize, int* _playerPosition)
{
    // char (*_output)[_windowSize[0]] = new char[_windowSize[0]][_windowSize[1]];
    float* _output = new float[_windowSize[0]];
    for (int i = 0; i < _windowSize[0]; i++)
    {
        _output[i] = 0.5f;
    }
    float _rayAngleStep = FOV * 1.f / _windowSize[0];
    int* _rayPosition;
    // cout << (int)(_rayPosition[3] * 1.0 / renderDistance * _windowSize[1]);
    
    // cout << "Ray angle, Ray distance, Column height, Symbol index, Symbol" << endl;
    for (int i = 0; i < _windowSize[0]; i++)
    {
        _rayPosition = RayCollisionDetection(AngleToRayDestination(_rayAngleStep * i, _playerPosition), _map, _mapSize, _playerPosition);
        // cout << (int)(_rayAngleStep * i) << " deg, " << _rayPosition[3] << ", " << (int)(_rayPosition[3] * 1.0 / renderDistance * _windowSize[1]) << ", " << (int)((1.0 - _rayPosition[3] * 1.0 / renderDistance) * (brightness.length() - 1)) << ", " << brightness[(int)((1.0 - _rayPosition[3] * 1.0 / renderDistance) * (brightness.length() - 1))] << "; ";
        _output[i] = _rayPosition[3] * 1.f / renderDistance;
        // cout << endl;
    }
    // cout << endl;

    return _output;
}
