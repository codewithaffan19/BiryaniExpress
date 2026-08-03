#pragma once

#include "raylib.h"

#include "../World/Map.h"

struct RayHit
{
    float distance;

    int side;

    int mapX;

    int mapY;

    float wallX;
};

class Raycaster
{
public:

    static RayHit CastRay(
        Vector2 position,
        Vector2 direction,
        const Map& map
    );
};