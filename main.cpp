#include <iostream>
#include <cmath>
using namespace std;

// Public variables init
string map[] = {
    "##########",
    "#   #     ",
    "# ### ####",
    "#        #",
    "###### # #",
    "#    # # #",
    "# ## # # #",
    "# #  # # #",
    "P #    # #",
    "##########"
};
int* playerPosition;
int displayResolution[2] = {32, 18};
int FOV = 120;
int renderDistance = 5;

int* AngleToRayDestination(int angle)
{
    static int _rayDestination[2];
    _rayDestination[0] = playerPosition[0] + cos(angle) * renderDistance;
    _rayDestination[1] = playerPosition[1] + sin(angle) * renderDistance;
    return _rayDestination;
}

int* RayCollisionDetection(int rayDestination[])
{
    float _rayStep[2];
    _rayStep[0] = (rayDestination[0] - playerPosition[0]) * 1.0 / renderDistance;
    _rayStep[1] = (rayDestination[1] - playerPosition[1]) * 1.0 / renderDistance;
    
    static int _rayPosition[2];
    _rayPosition[0] = -1;
    _rayPosition[1] = -1;
    for (int i = 0; i < renderDistance; i++)
    {
        _rayPosition[0] = playerPosition[0] + (int)(_rayStep[0] * i);
        _rayPosition[1] = playerPosition[1] + (int)(_rayStep[1] * i);
        if (_rayPosition[0] < 0 | _rayPosition[0] > map[0].length() | _rayPosition[1] < 0 | _rayPosition[1] > (sizeof(map) / sizeof(*map)))
        {
            break;
        }
        else
        {
            if (map[_rayPosition[1]][_rayPosition[0]] == '#')
            {
                break;
            }
        }
    }
    return _rayPosition;
    
}

int* GetPlayerPosition()
{
    static int _playerPosition[2] = {-1, -2};
    
    for (int y = 0; y < (sizeof(map) / sizeof(*map)); y++)
    {
        for (int x = 0; x < map[y].length(); x++)
        {
            // cout << map[y][x];
            if (map[y][x] == 'P')
            {
                _playerPosition[0] = x;
                _playerPosition[1] = y;
                break;
            }
            
        }
        if (_playerPosition[0] != -1)
        {
            break;
        }
        
        // cout << endl;
    }
    return _playerPosition;
}

int main()
{
    // Init
    playerPosition = GetPlayerPosition();
    cout << "Player position: " << playerPosition[0] << ", " << playerPosition[1] << endl;
    
    int _angle;
    bool _askForAngle = false;
    if (_askForAngle)
    {
        cout << "Enter angle: ";
        cin >> _angle;
    }
    else
    {
        _angle = 42; // Veritasium angle
    }
    int* _rayDestination = AngleToRayDestination(_angle);
    cout << "Ray destination at angle " << _angle << " is " << _rayDestination[0] << ", " << _rayDestination[1] << endl;

    int* _rayPosition = RayCollisionDetection(_rayDestination);
    cout << "Ray position is " << _rayPosition[0] << ", " << _rayPosition[1];

    return 0;
}