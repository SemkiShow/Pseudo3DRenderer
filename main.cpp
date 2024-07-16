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
int* playerCoords;
int displayResolution[2] = {32, 18};
int FOV = 120;
int renderDistance = 5;

int* AngleToRayDestination(int angle)
{
    static int _rayDestination[2];
    _rayDestination[0] = playerCoords[0] + cos(angle) * renderDistance;
    _rayDestination[1] = playerCoords[1] + sin(angle) * renderDistance;
    return _rayDestination;
}

int RayCollisionDetection(/* int destinationX, int destinationY */)
{

}

int* GetPlayerPosition()
{
    static int _playerCoords[2] = {-1, -2};
    
    for (int y = 0; y < (sizeof(map) / sizeof(*map)); y++)
    {
        for (int x = 0; x < map[y].length(); x++)
        {
            // cout << map[y][x];
            if (map[y][x] == 'P')
            {
                _playerCoords[0] = x;
                _playerCoords[1] = y;
                break;
            }
            
        }
        if (_playerCoords[0] != -1)
        {
            break;
        }
        
        // cout << endl;
    }
    return _playerCoords;
}

int main()
{
    // Init
    playerCoords = GetPlayerPosition();
    cout << "Player position: " << playerCoords[0] << ", " << playerCoords[1] << endl;

    

    return 0;
}