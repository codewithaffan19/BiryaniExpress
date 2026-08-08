#pragma once

#include "raylib.h"
#include <vector>

#include "../World/Map.h"
#include "../Core/Config.h"
#include "../Enemy.h"
#include "../Player/Player.h"
#include "../Renderer/TextureManager.h"
class Renderer
{
public:

    Renderer();
    void Draw(
        Vector2 playerPos,
        Vector2 playerDir,
        Vector2 cameraPlane,
        Map& map,
        const std::vector<Enemy>& enemies,
        Player player,
        bool insideMarket);
    TextureManager textures;
    Color* floorBuffer;
    Texture2D floorTexture;
    void LoadTextures();
    void UnloadTextures();
    void DrawDoorMarker(
        Vector2 playerPos,
        Vector2 playerDir,
        Vector2 cameraPlane,
        float Zbuffer[],
        Vector2 markerPos);
    void StartFadeIn();
    void StartFadeOut();
bool IsFadeFinished() const;

void UpdateFade(float dt);

void DrawFade();
private:

    float fadeAlpha = 0.0f;

    bool fadingIn = false;
    bool fadingOut = false;
    float cloudOffset = 0.0f;
    void DrawWallColumn(
        int screenX,
        float distance,
        int side,
        int tile,
        float wallX,
        float rayDirX,
        float rayDirY);

    void DrawSky(Vector2 playerDir);
    void DrawFloor(Vector2 PlayerPos, Vector2 PlayerDir,Vector2 CameraPlane);
    void DrawStreetFloor(
        Vector2 playerPos,
        Vector2 playerDir,
        Vector2 cameraPlane);

    void DrawMarketFloor(
        Vector2 playerPos,
        Vector2 playerDir,
        Vector2 cameraPlane);

    void DrawMarketRoof(
        Vector2 playerDir);
};