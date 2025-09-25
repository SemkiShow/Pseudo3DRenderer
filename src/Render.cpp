#include "Render.hpp"
#include "Settings.hpp"
#include <cmath>
#include <iostream>
#include <thread>

double rotationXOffset = -90 - fov / 2.0;
Vector2 windowSize = {16 * 50 * 2, 9 * 50 * 2};
Vector2 playerPosition = {0, 0};
std::vector<std::string> map;
std::vector<double> output;
size_t threadsNumber = std::fmax(std::thread::hardware_concurrency(), 1);

#ifndef M_PI
#define M_PI 3.1415926536
#endif

double DegreesToRadians(double degrees) { return degrees * M_PI / 180; }

int Raycast(double angle)
{
    Vector2 rayDestination;
    rayDestination.x = playerPosition.x + cos(angle) * renderDistance * scale;
    rayDestination.y = playerPosition.y + sin(angle) * renderDistance * scale;

    Vector2 rayStep;
    rayStep.x = (rayDestination.x - playerPosition.x) / renderDistance / scale;
    rayStep.y = (rayDestination.y - playerPosition.y) / renderDistance / scale;

    Vector3 ray;
    ray.x = 0;
    ray.y = 0;
    ray.z = 0; // Ray distance
    for (int i = 0; i < renderDistance * scale; i++)
    {
        ray.x = playerPosition.x + floor(rayStep.x * i);
        ray.y = playerPosition.y + floor(rayStep.y * i);
        if (ray.x < 0 || ray.x >= map[0].size() || ray.y < 0 || ray.y >= map.size())
        {
            ray.z = renderDistance * scale;
            break;
        }
        if (map[ray.y][ray.x] == '#')
        {
            break;
        }
        ray.z++;
    }
    return ray.z;
}

void GetPlayerPosition()
{
    std::cout << "The map size is " << map.size() << ", " << map[0].size() << std::endl;

    bool foundPlayer = false;
    for (size_t y = 0; y < map.size(); y++)
    {
        for (size_t x = 0; x < map[y].size(); x++)
        {
            if (map[y][x] == 'P')
            {
                playerPosition.x = x;
                playerPosition.y = y;
                foundPlayer = true;
                break;
            }
        }
        if (foundPlayer) break;
    }
}

void RenderFrame(int threadID)
{
    for (int i = threadID * windowSize.x / threadsNumber;
         i < (threadID + 1) * windowSize.x / threadsNumber; i++)
    {
        double angle = DegreesToRadians(rotationXOffset) - (DegreesToRadians(fov) / 2) +
                       (i * DegreesToRadians(fov) / windowSize.x);

        double rayLength = Raycast(angle);
        rayLength *= cos(angle - DegreesToRadians(rotationXOffset));
        output[i] = 1 - rayLength * 1.0 / renderDistance / scale;
    }
}

std::vector<double> GetFrame()
{
    output.resize(windowSize.x, 0);
#if defined(PLATFORM_WEB)
    threadsNumber = 1;
    RenderFrame(0);
#else
    std::vector<std::thread> threads;
    for (size_t i = 0; i < threadsNumber; i++)
        threads.push_back(std::thread(RenderFrame, i));
    for (size_t i = 0; i < threadsNumber; i++)
        threads[i].join();
#endif
    return output;
}
