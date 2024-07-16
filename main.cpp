#include <iostream>
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

int* DegreesToDestination(/* int playerX, int playerY, int degree, int renderDistance */)
{

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