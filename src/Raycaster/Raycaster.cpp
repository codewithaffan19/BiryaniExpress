#include "Raycaster.h"

#include <cmath>

RayHit Raycaster::CastRay(
    Vector2 position,
    Vector2 direction,
    const Map& map)
{
    int mapX = (int)position.x;
    int mapY = (int)position.y;

    float deltaDistX =
        (direction.x == 0.0f)
        ? 1e30f
        : std::abs(1.0f / direction.x);

    float deltaDistY =
        (direction.y == 0.0f)
        ? 1e30f
        : std::abs(1.0f / direction.y);

    int stepX;
    int stepY;

    float sideDistX;
    float sideDistY;

    if (direction.x < 0)
    {
        stepX = -1;

        sideDistX =
            (position.x - mapX) *
            deltaDistX;
    }
    else
    {
        stepX = 1;

        sideDistX =
            (mapX + 1.0f - position.x) *
            deltaDistX;
    }

    if (direction.y < 0)
    {
        stepY = -1;

        sideDistY =
            (position.y - mapY) *
            deltaDistY;
    }
    else
    {
        stepY = 1;

        sideDistY =
            (mapY + 1.0f - position.y) *
            deltaDistY;
    }

    bool hit = false;

    int side = 0;

    while (!hit)
    {
        if (sideDistX < sideDistY)
        {
            sideDistX += deltaDistX;

            mapX += stepX;

            side = 0;
        }
        else
        {
            sideDistY += deltaDistY;

            mapY += stepY;

            side = 1;
        }

        if (map.GetCell(mapY, mapX) != 0)
            hit = true;
    }

    float distance;

    if (side == 0)
        distance = sideDistX - deltaDistX;
    else
        distance = sideDistY - deltaDistY;

    float wallX;

    if (side == 0)
        wallX =
        position.y +
        distance * direction.y;
    else
        wallX =
        position.x +
        distance * direction.x;

    wallX -= floorf(wallX);

    RayHit ray;

    ray.distance = distance;
    ray.side = side;
    ray.mapX = mapX;
    ray.mapY = mapY;
    ray.wallX = wallX;

    return ray;
}