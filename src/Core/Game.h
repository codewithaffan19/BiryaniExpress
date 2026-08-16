#pragma once

#include <vector>

#include "../Player/Player.h"
#include "../World/Map.h"
#include "../Renderer/Renderer.h"
#include "../Core/InputManager.h"
#include "../Enemy.h"
#include "../NPC/NPC.h"
#include "../Story/Story.h"
#include "../Menu/Menu.h"
#include "../Cutscene/Cutscene.h"
#include "../Audio/SoundManager.h"
class Game
{
public:
    Game();

    void Run();

private:
    void Initialize();
    void Update();
    void Draw();
    void Shutdown();
    void CheckSpoonCollosion(Player& a, Enemy& E1);
    void ResetGame();

    // Switches the active world partition AND its matching ambience
    // (Street / Market / Neon) in one call.
    void SetPartitionAndSound(TextureManager::Partition partition);
private:
    Menu menu;

    Cutscene cutscene;
    Cutscene neonCutscene;

    SoundManager soundManager;

    // How long the player has been continuously moving (WASD) —
    // drives the footstep sound cadence.
    float footstepTimer = 0.0f;

    bool neonCutsceneActive = false;
    bool winScreenActive = false;

    Texture2D winTexture{};

    float winZoom = 0.5f;
    bool gameStarted = false;
    bool running = false;
    bool storyRestrictionWarning = false;
    Player player;
    Map map;

    Renderer renderer;

    InputManager input;
    Enemy Police;
    std::vector<Enemy> enemies;
    std::vector<NPC> npcs;
    Story story;
    bool insideMarket = false;
    bool insideNeon = false;
    bool teleportPending = false;

    // Tracks whether we've already enabled the cursor for the
    // current Game Over screen, so it's only re-centered once
    // instead of every frame (was causing the cursor to look
    // "stuck" at screen-center while the restart buttons were up).
    bool gameOverCursorSet = false;

    Vector2 outsideDoor = { 8.5f, 2.5f };
    Vector2 insideDoor = { 11.5f, 2.5f };
    Vector2 outsideNeonDoor = { 22.5f, 6.5f };
    Vector2 insideNeonDoor = { 22.5f, 10.5f };
};