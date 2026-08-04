#pragma once

#include "raylib.h"
#include <vector>

#include "../World/Map.h"
#include "../Core/Config.h"
#include "../Enemy.h"
struct RayHit
{
    float distance;

    int side;
};

class Renderer
{
public:

    Renderer();

    void Draw(
        Vector2 playerPos,
        Vector2 playerDir,
        Vector2 cameraPlane,
        Map& map,
        const std::vector<Enemy>& enemies);
   
private:



    void DrawWallColumn(
        int screenX,
        float distance,
        int side);

    void DrawSky();

    void DrawFloor();

};