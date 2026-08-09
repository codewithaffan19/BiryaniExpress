#pragma once

#include <vector>

#include "raylib.h"
#include "../World/Map.h"
#include "../Core/Config.h"
#include "../Enemy.h"
#include "../Player/Player.h"
#include "../Renderer/TextureManager.h"
#include "../NPC/NPC.h"

class Renderer
{
public:

    Renderer();
    void DrawHud(Player&p);
    void DrawInventoryHUD(Player& p);
    void Draw(
        Vector2 playerPos,
        Vector2 playerDir,
        Vector2 cameraPlane,
        Map& map,
        const std::vector<Enemy>& enemies,
        const std::vector<NPC>& npcs,
        Player player,
        bool insideMarket
    );

    TextureManager textures;
    Color* floorBuffer = nullptr;
    Texture2D floorTexture{};

    void LoadTextures();
    void UnloadTextures();

    void DrawDoorMarker(
        Vector2 playerPos,
        Vector2 playerDir,
        Vector2 cameraPlane,
        float Zbuffer[],
        Vector2 markerPos
    );

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
        float rayDirY
    );

    void DrawSky(Vector2 playerDir);

    void DrawFloor(
        Vector2 playerPos,
        Vector2 playerDir,
        Vector2 cameraPlane
    );

    void DrawStreetFloor(
        Vector2 playerPos,
        Vector2 playerDir,
        Vector2 cameraPlane
    );

    void DrawMarketFloor(
        Vector2 playerPos,
        Vector2 playerDir,
        Vector2 cameraPlane
    );
    void DrawFountain(
        Vector2 fountainPos,
        Vector2 playerPos,
        Vector2 playerDir,
        Vector2 cameraPlane,
        float Zbuffer[]
    );
    void DrawMarketRoof(Vector2 playerDir);
};