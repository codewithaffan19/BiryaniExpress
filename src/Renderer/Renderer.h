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
    struct GameOverButtons {
        Rectangle retry;
        Rectangle mainMenu;
        Rectangle exit;
    };

    GameOverButtons GetGameOverButtonRects();

    void DrawHud(Player& p);
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

    // ============================================================
    // FULLSCREEN / VIRTUAL RESOLUTION
    //
    // The 3D view, HUD, Win screen, and Game Over screen are all
    // rendered into this fixed Config::SCREEN_WIDTH x SCREEN_HEIGHT
    // texture (gameView), which Game::Draw() then scales up to fill
    // the real (fullscreen) window in one blit via
    // DrawGameViewToScreen(). Every existing draw call inside that
    // group keeps using Config::SCREEN_WIDTH/HEIGHT exactly as
    // before — this is what lets fullscreen work at any monitor
    // resolution without re-deriving any layout math, and keeps
    // performance independent of monitor resolution (nothing here
    // scales with window size — only the final blit does).
    //
    // Menu / Cutscene / Pause already use GetScreenWidth()/Height()
    // dynamically, so they draw directly to the real backbuffer,
    // outside this texture, unaffected.
    // ============================================================

    RenderTexture2D gameView{};

    // Draws gameView scaled (letterboxed if the monitor's aspect
    // ratio isn't 16:9) to fill the real window. Call after
    // EndTextureMode(), before the outer EndDrawing().
    void DrawGameViewToScreen();

    // Converts a real (window-space) mouse position, as returned by
    // GetMousePosition(), into gameView's virtual Config::SCREEN_
    // WIDTH/HEIGHT space. Needed anywhere gameplay code hit-tests
    // the mouse against something drawn inside gameView (currently
    // just the Game Over buttons).
    static Vector2 GetVirtualMousePosition();

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

    // Which destination a fade transition is covering, so DrawFade()
    // can show the matching loading-screen background image.
    enum class LoadingTransition
    {
        None,
        Market,
        Neon
    };

    void StartFadeIn(LoadingTransition transition);
    void StartFadeOut();

    bool IsFadeFinished() const;
    void UpdateFade(float dt);
    void DrawFade();
    void DrawGameOver();
private:

    float fadeAlpha = 0.0f;
    TextureManager::Partition GetCurrentPartition() const;
    bool fadingIn = false;
    bool fadingOut = false;
    Texture2D Missionpouch[2];
    Texture2D Weaponpouch[3];
    float cloudOffset = 0.0f;

    LoadingTransition currentTransition = LoadingTransition::None;
    Texture2D loadingMarketTex{};
    Texture2D loadingNeonTex{};

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