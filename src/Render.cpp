#include "Render.hpp"

int FOV = 90;
double rotationXOffset = -90-FOV/2;
int rotationSensitivity = 200;
int scale = 10;
int movementSensitivity = 2;
int renderDistance = 20;
int windowSize[2] = {16*50*2, 9*50*2};
double playerPosition[2] = {-1, -2};
std::vector<std::string> map;
double* output = new double[windowSize[0]];
int threadsNumber = std::max(std::thread::hardware_concurrency(), (unsigned int)1);

#define PI 3.1415926536

double DegreesToRadians(double degrees)
{
    return degrees * PI / 180;
}

int Raycast(double angle)
{
    int rayDestination[2];
    rayDestination[0] = (int)playerPosition[0] + (int)(cos(angle) * renderDistance * scale);
    rayDestination[1] = (int)playerPosition[1] + (int)(sin(angle) * renderDistance * scale);

    double _rayStep[2];
    _rayStep[0] = (rayDestination[0] - (int)playerPosition[0]) * 1.0 / renderDistance / scale;
    _rayStep[1] = (rayDestination[1] - (int)playerPosition[1]) * 1.0 / renderDistance / scale;
    
    int rayPosition[3];
    rayPosition[0] = 0;
    rayPosition[1] = 0;
    rayPosition[3] = 0; // Ray distance
    for (int i = 0; i < renderDistance * scale; i++)
    {
        rayPosition[0] = (int)playerPosition[0] + (int)(_rayStep[0] * i);
        rayPosition[1] = (int)playerPosition[1] + (int)(_rayStep[1] * i);
        if (rayPosition[0] < 0 | rayPosition[0] >= map[0].size() | rayPosition[1] < 0 | rayPosition[1] >= map.size())
        {
            rayPosition[3] = renderDistance * scale;
            break;
        }
        if (map[rayPosition[1]][rayPosition[0]] == '#')
        {
            break;
        }
        rayPosition[3]++;
    }
    return rayPosition[3];
}

void GetPlayerPosition()
{
    std::cout << "The map size is " << map.size() << ", " << map[0].size() << std::endl;
    
    bool foundPlayer = false;
    for (int y = 0; y < map.size(); y++)
    {
        for (int x = 0; x < map[y].size(); x++)
        {
            if (map[y][x] == 'P')
            {
                playerPosition[0] = x;
                playerPosition[1] = y;
                foundPlayer = true;
                break;
            }
        }
        if (foundPlayer) break;
    }
}

void RenderFrame(int threadID)
{
    for (int i = threadID * windowSize[0] / threadsNumber; i < (threadID + 1) * windowSize[0] / threadsNumber; i++)
    {
        double angle = DegreesToRadians(rotationXOffset) - (DegreesToRadians(FOV) / 2) + (i * DegreesToRadians(FOV) / windowSize[0]);

        int rayLength = Raycast(angle);
        rayLength *= cos(angle - DegreesToRadians(rotationXOffset));
        output[i] = 1 - rayLength * 1.0 / renderDistance / scale;
    }
}
