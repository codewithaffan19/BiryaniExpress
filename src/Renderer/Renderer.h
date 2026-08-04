#pragma once

#include "raylib.h"
#include <vector>

#include "../World/Map.h"
#include "../Core/Config.h"
#include "../Enemy.h"

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
    Texture2D wallTexture;
    void LoadTextures();
    void UnloadTextures();
private:



    void DrawWallColumn(
        int screenX,
        float distance,
        int side,
        int tile,
        float wallX);

    void DrawSky();

    void DrawFloor();
};