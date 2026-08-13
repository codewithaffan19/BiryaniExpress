#pragma once

#include <vector>

#include "raylib.h"
#include "../World/Map.h"
#include "../Core/Config.h"
#include "../Enemy.h"
#include "../Player/Player.h"
#include "../Renderer/TextureManager.h"
#include "../NPC/NPC.h"
#include "../Story/Story.h"
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
        std::vector<Enemy>& enemies,
        std::vector<NPC>& npcs,
        Player& player,
        bool insideMarket,
        bool nearDoor,
        Story& story,
        bool insideNeon
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
        Vector2 markerPos,
        Map& map
    );

    void StartFadeIn();
    void StartFadeOut();

    bool IsFadeFinished() const;
    void UpdateFade(float dt);
    void DrawFade();

private:

    float fadeAlpha = 0.0f;
    TextureManager::Partition GetCurrentPartition() const;
    bool fadingIn = false;
    bool fadingOut = false;
    Texture2D Missionpouch[2];
    Texture2D Weaponpouch[3];
    float cloudOffset = 0.0f;

    void DrawWallColumn(
        int screenX,
        float distance,
        int side,
        int tile,
        float wallX,
        float rayDirX,
        float rayDirY,
        bool nearDoor);

    void DrawSky(Vector2 playerDir);

    void DrawFloor(
        Vector2 playerPos,
        Vector2 playerDir,
        Vector2 cameraPlane
    );

    void DrawWideFloor(
        Vector2 playerPos,
        Vector2 playerDir,
        Vector2 cameraPlane,
        Map& map
    );

    void DrawMarketFloor(
        Vector2 playerPos,
        Vector2 playerDir,
        Vector2 cameraPlane
    );
    void DrawNeonNightFloor(
        Vector2 playerPos,
        Vector2 playerDir,
        Vector2 cameraPlane
    );

    void DrawNeonNightSky(Vector2 playerDir);
    void DrawMarketRoof(Vector2 playerDir);
};