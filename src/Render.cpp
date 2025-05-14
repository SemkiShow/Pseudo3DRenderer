#include "Render.hpp"

int FOV = 90;
double rotationXOffset = -90-FOV/2;
int rotationSensitivity = 200;
int scale = 10;
int movementSensitivity = 2;
int renderDistance = 20 * scale;
int windowSize[2] = {16*50*2, 9*50*2};
double playerPosition[2] = {-1, -2};
std::vector<std::string> map;

double DegreesToRadians(double degrees)
{
    return degrees * (3.1415926536 / 180);
}

int* AngleToRayDestination(double angle)
{
    static int rayDestination[2];
    rayDestination[0] = (int)playerPosition[0] + (int)(cos(DegreesToRadians(angle)) * renderDistance);
    rayDestination[1] = (int)playerPosition[1] + (int)(sin(DegreesToRadians(angle)) * renderDistance);
    return rayDestination;
}

int* RayCollisionDetection(int* rayDestination)
{
    double _rayStep[2];
    _rayStep[0] = (rayDestination[0] - (int)playerPosition[0]) * 1.0 / renderDistance;
    _rayStep[1] = (rayDestination[1] - (int)playerPosition[1]) * 1.0 / renderDistance;
    
    static int rayPosition[3];
    rayPosition[0] = 0;
    rayPosition[1] = 0;
    rayPosition[3] = 0; // Ray distance
    for (int i = 0; i < renderDistance; i++)
    {
        rayPosition[0] = (int)playerPosition[0] + (int)(_rayStep[0] * i);
        rayPosition[1] = (int)playerPosition[1] + (int)(_rayStep[1] * i);
        // std::cout << map[rayPosition[1]][rayPosition[0]] << "; ";
        if (rayPosition[0] < 0 | rayPosition[0] >= map[0].size() | rayPosition[1] < 0 | rayPosition[1] >= map.size())
        {
            rayPosition[3] = renderDistance;
            break;
        }
        if (map[rayPosition[1]][rayPosition[0]] == '#')
        {
            break;
        }
        rayPosition[3]++;
    }
    return rayPosition;
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

double* RenderFrame()
{
    double* output = new double[windowSize[0]];
    for (int i = 0; i < windowSize[0]; i++)
    {
        output[i] = 0.5;
    }
    double rayAngleStep = FOV * 1.0 / windowSize[0];
    int* rayPosition;
    
    for (int i = 0; i < windowSize[0]; i++)
    {
        // int* forward = AngleToRayDestination(rotationXOffset);
        // double right[2] = {forward[1] * 1.0, -forward[0] * 1.0};
        // double halfWidth = tan(DegreesToRadians(FOV / 2));
        // int rayDestination[2] = {0, 0};
        // double offset = ((i * 2.0 / (windowSize[0] - 1.0)) - 1.0) * halfWidth;
        // rayDestination[0] = forward[0] + offset * right[0];
        // rayDestination[1] = forward[1] + offset * right[1];

        rayPosition = RayCollisionDetection(/* rayDestination */AngleToRayDestination(rayAngleStep * i + rotationXOffset));
        output[i] = 1 - rayPosition[3] * 1.0 / renderDistance;
        // output[i] *= 0.95;
    }

    return output;
}
