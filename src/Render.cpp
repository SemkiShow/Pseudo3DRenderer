#include <iostream>
#include <cmath>
#include <random>
#include <vector>
using namespace std;

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#elif defined(__linux__)
#include <sys/ioctl.h>
#endif // Windows/Linux

void GetTerminalSize(int& width, int& height) {
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    width = (int)(csbi.srWindow.Right-csbi.srWindow.Left+1);
    height = (int)(csbi.srWindow.Bottom-csbi.srWindow.Top+1);
#elif defined(__linux__)
    struct winsize w;
    ioctl(fileno(stdout), TIOCGWINSZ, &w);
    width = (int)(w.ws_col);
    height = (int)(w.ws_row);
#endif // Windows/Linux
}


// Public variables init
string brightness = " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";
// string brightness = "#";
vector<string> map;
string map1[] = {
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
string map2[] = {
    "   #",
    "    ",
    "P  #",
    "    ",
    "   #"
};
string map3[] = {
    "     #######   ",
    "    #    ## #  ",
    "   #   #     # ",
    "   # ######### ",
    "   # ##        ",
    " ### ## # # ###",
    "#    ## # # # #",
    "# ##    #     #",
    "#  # # ####### ",
    "## # #       # ",
    "#  # ### ### # ",
    "# ##   # ### # ",
    "P ### ## # # # ",
    "# ##  #  #   # ",
    " ### ####### # ",
    "   # # # #   # ",
    "   # # # ### # ",
    "   #   # #   # ",
    "    ###   ###  "
};
string map4[] = {
    "     #######   ",
    "    #       #  ",
    "   #         # ",
    "   #  ######## ",
    "   #  #       #",
    " ###  #       #",
    "#  #  #       #",
    "#  #  #       #",
    "#  #   ####### ",
    "#  #         # ",
    "#  #         # ",
    "#  #    P    # ",
    "#  #         # ",
    "#  #         # ",
    " ###   ####  # ",
    "   #   # #   # ",
    "   #   # #   # ",
    "   #   # #   # ",
    "    ###   ###  "
};
int* playerPosition;
int displayResolution[2] = {32, 18};
int FOV = 720;
int renderDistance = 5;
int terminalSize[2];
random_device dev;
mt19937 rng(dev());
uniform_int_distribution<mt19937::result_type> dist9(0, 9); // distribution in range [0, 9]

float DegreesToRadians(float _degrees) 
{
    return _degrees * (3.141592654 / 180);
}

int* AngleToRayDestination(float _angle)
{
    static int _rayDestination[2];
    _rayDestination[0] = playerPosition[0] + (int)(cosf(DegreesToRadians(_angle)) * renderDistance);
    _rayDestination[1] = playerPosition[1] + (int)(sinf(DegreesToRadians(_angle)) * renderDistance);
    return _rayDestination;
}

int* RayCollisionDetection(int* _rayDestination, string* _map, int _mapSize)
{
    float _rayStep[2];
    _rayStep[0] = (_rayDestination[0] - playerPosition[0]) * 1.0 / renderDistance;
    _rayStep[1] = (_rayDestination[1] - playerPosition[1]) * 1.0 / renderDistance;
    
    static int _rayPosition[3];
    _rayPosition[0] = 0;
    _rayPosition[1] = 0;
    _rayPosition[3] = 0; // Ray distance
    for (int i = 0; i < renderDistance; i++)
    {
        _rayPosition[0] = playerPosition[0] + (int)(_rayStep[0] * i);
        _rayPosition[1] = playerPosition[1] + (int)(_rayStep[1] * i);
        // cout << _map[_rayPosition[1]][_rayPosition[0]] << "; ";
        if (_rayPosition[0] < 0 | _rayPosition[0] > _map[0].length() | _rayPosition[1] < 0 | _rayPosition[1] > _mapSize)
        {
            // _rayPosition[3] = renderDistance;
            // break;
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

char** Render(string* _map, int _mapSize)
{
    // char (*_output)[terminalSize[0]] = new char[terminalSize[0]][terminalSize[1]];
    char** _output = new char*[terminalSize[1]];
    for (int i = 0; i < terminalSize[1]; i++) _output[i] = new char[terminalSize[0]];
    /* for (int y = 0; y < terminalSize[1]; y++)
    {
        for (int x = 0; x < terminalSize[0]; x++)
        {
            _output[y][x] = dist9(rng) + 48;
        }
    } */
    float _rayAngleStep = FOV * 1.0 / terminalSize[0];
    int* _rayPosition;
    // cout << (int)(_rayPosition[3] * 1.0 / renderDistance * terminalSize[1]);
    
    // cout << "Ray angle, Ray distance, Column height, Symbol index, Symbol" << endl;
    for (int i = 0; i < terminalSize[0]; i++)
    {
        _rayPosition = RayCollisionDetection(AngleToRayDestination(_rayAngleStep * i), _map, _mapSize);
        // cout << (int)(_rayAngleStep * i) << " deg, " << _rayPosition[3] << ", " << (int)(_rayPosition[3] * 1.0 / renderDistance * terminalSize[1]) << ", " << (int)((1.0 - _rayPosition[3] * 1.0 / renderDistance) * (brightness.length() - 1)) << ", " << brightness[(int)((1.0 - _rayPosition[3] * 1.0 / renderDistance) * (brightness.length() - 1))] << "; ";
        for (int y = 0; y < (int)(_rayPosition[3] * 1.0 / renderDistance * terminalSize[1]); y++)
        {
            _output[y][i] = ' ';
            // cout << ' ';
        }
        for (int y = (int)(_rayPosition[3] * 1.0 / renderDistance * terminalSize[1]); y < terminalSize[1]; y++)
        {
            _output[y][i] = brightness[(int)((1.0 - (_rayPosition[3] * 1.0 / renderDistance)) * (brightness.length() - 1))];
            // cout << brightness[(int)((1.0 - _rayPosition[3] * 1.0 / renderDistance - 1) * 1.0 * brightness.length() - 1)];
        }
        // cout << endl;
    }
    cout << endl;

    return _output;
}

int main()
{
    // Init
    // system("CLS");
    for (int i = 0; i < (sizeof(map2) / sizeof(*map2)); i++)
    {
        map.push_back(map2[i]);
    }
    /* cout << "map2:" << endl;
    for (int y = 0; y < (sizeof(map2) / sizeof(map2[0])); y++)
    {
        for (int x = 0; x < map2[y].length(); x++)
        {
            cout << map2[y][x];
        }
        cout << endl;
    }
    cout << "map:" << endl;
    for (int y = 0; y < (sizeof(map) / sizeof(map[0])); y++)
    {
        for (int x = 0; x < map[y].length(); x++)
        {
            cout << map[y][x];
        }
        cout << endl;
    } */
    string _map[map.size()];
    for (int i = 0; i < map.size(); i++)
    {
        _map[i] = map[i];
    }
    int _mapSize = sizeof(_map) / sizeof(*_map);
    /* cout << "_map:" << endl;
    for (int y = 0; y < (sizeof(_map) / sizeof(_map[0])); y++)
    {
        for (int x = 0; x < _map[y].length(); x++)
        {
            cout << _map[y][x];
        }
        cout << endl;
    } */

    cout << "<Debug info>" << endl;

    cout << brightness << endl;
    // cout << 3.141592654 << endl;  

    GetTerminalSize(terminalSize[0], terminalSize[1]);
    cout << "Terminal size is " << terminalSize[0] << ", " << terminalSize[1] << endl;

    playerPosition = GetPlayerPosition(_map, _mapSize);
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
        _angle = 42.0; // Veritasium angle
    }
    int* _rayDestination = AngleToRayDestination(_angle);
    cout << "Ray destination at angle " << _angle << " is " << _rayDestination[0] << ", " << _rayDestination[1] << endl;

    int* _rayPosition = RayCollisionDetection(_rayDestination, _map, _mapSize);
    cout << "Ray position is " << _rayPosition[0] << ", " << _rayPosition[1] << ". The ray has travelled " << _rayPosition[3] << " m" << endl;

    cout << "</Debug info>" << endl;

    char** _output;
    _output = Render(_map, _mapSize);
    for (int y = 0; y < terminalSize[1]; y++)
    {
        for (int x = 0; x < terminalSize[0]; x++)
        {
            cout << _output[y][x];
        }
        cout << endl;
    }

    return 0;
}
