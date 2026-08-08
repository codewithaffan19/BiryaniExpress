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
        Player player);
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
        float wallX);

    void DrawSky(Vector2 playerDir);
    void DrawFloor(Vector2 PlayerPos, Vector2 PlayerDir,Vector2 CameraPlane);
};