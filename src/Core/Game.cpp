#include "Game.h"
#include "raylib.h"
#include "Config.h"
#include "InputManager.h"
#include <cmath>
#include "../Renderer/Renderer.h"
#include "../World/Map.h"
#include "../Renderer/TextureManager.h"

#include "raymath.h"
#include "../World/Collision.h"
Game::Game()
{
    running = true;
    
}

void Game::Initialize()
{
    InitWindow(
        Config::SCREEN_WIDTH,
        Config::SCREEN_HEIGHT,
        "BiryaniExpress"
    );

    // ============================================================
    // AUDIO
    // ============================================================

    InitAudioDevice();

    if (!IsAudioDeviceReady())
    {
        TraceLog(
            LOG_ERROR,
            "Failed to initialize audio device!"
        );
    }

    renderer.LoadTextures();
    renderer.textures.SetCurrentPartition(
        TextureManager::Partition::Street
    );
    editor.SetTextureManager(&renderer.textures);

    DisableCursor();
    SetTargetFPS(Config::TARGET_FPS);

    // Hand
    Image hand = LoadImage("../assets/textures/hand.png");
    ImageColorReplace(&hand, MAGENTA, BLANK);

    player.handTex = LoadTextureFromImage(hand);

    UnloadImage(hand);

    // ==========================================
    // ENEMIES
    // ==========================================

    enemies.clear();

    Enemy BurgerBoy;
    BurgerBoy.spriteSheet =
        LoadTexture("../assets/textures/Boy.png");
    BurgerBoy.totalframes = 4;
    BurgerBoy.moveSpeed = 0.8f;
    BurgerBoy.position = { 13.0f, 14.0f };

    Enemy AngryUncle;
    AngryUncle.spriteSheet =
        LoadTexture("../assets/textures/uncle.png");
    AngryUncle.position = { 7.0f, 10.0f };
    AngryUncle.totalframes = 5;
    AngryUncle.moveSpeed = 0.7f;

    Enemy AngryUncle2;
    AngryUncle2.spriteSheet =
        LoadTexture("../assets/textures/uncle.png");
    AngryUncle2.position = { 17.0f, 16.0f };
    AngryUncle2.totalframes = 5;
    AngryUncle2.moveSpeed = 0.7f;

    Enemy Thief;
    Thief.spriteSheet =
        LoadTexture("../assets/textures/Chor.png");
    Thief.position = { 1.0f, 7.0f };
    Thief.totalframes = 2;
    Thief.moveSpeed = 0.9f;

    Enemy Thief2;
    Thief2.spriteSheet =
        LoadTexture("../assets/textures/Chor2.png");
    Thief2.position = { 14.0f, 16.0f };
    Thief2.totalframes = 2;
    Thief2.moveSpeed = 0.9f;

    enemies.push_back(Thief);
    enemies.push_back(AngryUncle);
    enemies.push_back(Thief2);
    enemies.push_back(BurgerBoy);
    enemies.push_back(AngryUncle2);

    // ==========================================
    // MAP
    // ==========================================

    map.LoadMap("../assets/maps/test.txt");

    // ==========================================
    // NPCs
    // ==========================================

    npcs.clear();

    NPC afc;
    afc.myItem = ITEM_AFC;
    if (!afc.Load(
        "AFC Waiter",
        "../assets/textures/AFC_waiter.png",
        { 13.5f, 1.5f }))
    {
        TraceLog(LOG_ERROR, "Failed to load AFC Waiter");
    }
    npcs.push_back(afc);

    NPC mike;
    mike.myItem = ITEM_MIKE;
    if (!mike.Load(
        "MIKE Waiter",
        "../assets/textures/MIKE_waiter.png",
        { 13.5f, 3.5f }))
    {
        TraceLog(LOG_ERROR, "Failed to load MIKE Waiter");
    }
    npcs.push_back(mike);

    NPC clinex;
    clinex.myItem = ITEM_CLINIX;
    if (!clinex.Load(
        "CLINEX Waiter",
        "../assets/textures/CLINEX_waiter.png",
        { 19.5f, 3.5f }))
    {
        TraceLog(LOG_ERROR, "Failed to load CLINEX Waiter");
    }
    npcs.push_back(clinex);

    NPC drumble;
    drumble.myItem = ITEM_DRUMBLE;
    if (!drumble.Load(
        "DRUMBLE Waiter",
        "../assets/textures/DRUMBLE_waiter.png",
        { 17.5f, 1.5f }))
    {
        TraceLog(LOG_ERROR, "Failed to load DRUMBLE Waiter");
    }
    npcs.push_back(drumble);


    //==========
    //Jack
    //=========

    NPC jack;

    if (!jack.Load(
        "Jack",
        "../assets/textures/jack.png",
        { 22.5f, 3.5f }))
    {
        TraceLog(LOG_ERROR, "Failed to load Jack");
    }

    npcs.push_back(jack);

}
void Game::CheckSpoonCollosion(Player& p, Enemy& E)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        float dx = E.position.x - p.position.x;
        float dy = E.position.y - p.position.y;

        float dis = sqrt((dx * dx) + (dy * dy));

        if (dis < 1.5f && dis > 0.001f)
        {
            float normX = dx / dis;
            float normY = dy / dis;

            float result =
                (p.GetDirection().x * normX) +
                (p.GetDirection().y * normY);

            if (result > 0.85f)
            {
                p.hitmessagetimer = 30;
                E.health -= 20;
            }
        }
    }
}
void Game::Update()
{
    float dt = GetFrameTime();

    input.Update();

    // ==========================================
    // EDITOR TOGGLE
    // ==========================================

    if (IsKeyPressed(KEY_TAB))
    {
        editorMode = !editorMode;

        if (editorMode)
            EnableCursor();
        else
            DisableCursor();
    }

    // ==========================================
    // GAME / EDITOR UPDATE
    // ==========================================

    if (editorMode)
    {
        editor.Update(map);
    }
    else
    {
        player.Update(dt, input, map);

        for (int i = 0; i < enemies.size(); i++)
        {
            enemies[i].update(player, map);

            CheckPlayerEnemyCollision(
                player,
                enemies[i],
                map
            );

            CheckSpoonCollosion(
                player,
                enemies[i]
            );
        }

        for (int i = 0; i < npcs.size(); i++)
        {
            CheckPlayerNPCCollision(
                player,
                npcs[i],
                map
            );
        }
    }

    // ==========================================
    // UPDATE NPCs
    // ==========================================

    Vector2 playerPos = player.GetPosition();

    for (NPC& npc : npcs)
    {
        npc.Update(playerPos, player);
    }

    // ==========================================
    // DOOR SYSTEM
    // ==========================================

    if (!editorMode)
    {
        // --------------------------------------
        // INSIDE MARKET
        // --------------------------------------

        if (insideMarket)
        {
            float dist =
                Vector2Distance(
                    playerPos,
                    insideDoor
                );

            if (dist < 1.2f && IsKeyPressed(KEY_E))
            {
                renderer.StartFadeIn();
                teleportPending = true;
            }
        }

        // --------------------------------------
        // INSIDE NEON
        // --------------------------------------

        else if (insideNeon)
        {
            float dist =
                Vector2Distance(
                    playerPos,
                    insideNeonDoor
                );

            if (dist < 1.2f && IsKeyPressed(KEY_E))
            {
                renderer.StartFadeIn();
                teleportPending = true;
            }
        }

        // --------------------------------------
        // OUTSIDE
        // --------------------------------------

        else
        {
            float marketDist =
                Vector2Distance(
                    playerPos,
                    outsideDoor
                );

            float neonDist =
                Vector2Distance(
                    playerPos,
                    outsideNeonDoor
                );

            if ((marketDist < 1.2f ||
                neonDist < 1.2f) &&
                IsKeyPressed(KEY_E))
            {
                renderer.StartFadeIn();
                teleportPending = true;
            }
        }
    }

    // ==========================================
    // ESCAPE
    // ==========================================

    if (input.IsKeyPressed(KEY_ESCAPE))
        running = false;
}


void Game::Draw()
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    // ==========================================
    // EDITOR
    // ==========================================

    if (editorMode)
    {
        editor.Draw(map);
    }
    else
    {
        // ==========================================
        // PLAYER CAMERA DATA
        // ==========================================

        Vector2 playerPos =
            player.GetPosition();

        Vector2 playerDir =
            player.GetDirection();

        Vector2 cameraPlane =
            player.GetCameraPlane();

        // ==========================================
        // 3D WORLD
        // ==========================================
        Vector2 targetDoor;

        if (insideMarket)
        {
            targetDoor = insideDoor;
        }
        else if (insideNeon)
        {
            targetDoor = insideNeonDoor;
        }
        else
        {
            float marketDist =
                Vector2Distance(
                    playerPos,
                    outsideDoor
                );

            float neonDist =
                Vector2Distance(
                    playerPos,
                    outsideNeonDoor
                );

            targetDoor =
                (neonDist < marketDist)
                ? outsideNeonDoor
                : outsideDoor;
        }

        float doorDistance =
            Vector2Distance(
                playerPos,
                targetDoor
            );

        bool nearDoor =
            doorDistance < 1.2f;
        renderer.Draw(
            playerPos,
            playerDir,
            cameraPlane,
            map,
            enemies,
            npcs,
            player,
            insideMarket,
            nearDoor
        );
        // ==========================================
        // DOOR INTERACTION TEXT
        // ==========================================

        float distOutside =
            Vector2Distance(
                playerPos,
                outsideDoor
            );

        float distInside =
            Vector2Distance(
                playerPos,
                insideDoor
            );

        if (insideMarket || insideNeon)
        {
            if (Vector2Distance(playerPos,
                insideMarket ? insideDoor : insideNeonDoor) < 1.2f)
            {
                DrawText(
                    "Press E to Exit",
                    Config::SCREEN_WIDTH / 2 - 120,
                    Config::SCREEN_HEIGHT - 80,
                    24,
                    YELLOW
                );
            }
        }
        else
        {
            float marketDist =
                Vector2Distance(playerPos, outsideDoor);

            float neonDist =
                Vector2Distance(playerPos, outsideNeonDoor);

            if (marketDist < 1.2f || neonDist < 1.2f)
            {
                DrawText(
                    "Press E to Enter",
                    Config::SCREEN_WIDTH / 2 - 120,
                    Config::SCREEN_HEIGHT - 80,
                    24,
                    YELLOW
                );
            }
        }
    }

    // ==========================================
    // FADE
    // ==========================================

    renderer.UpdateFade(GetFrameTime());

    if (teleportPending &&
        renderer.IsFadeFinished())
    {
        teleportPending = false;

        // ==========================================
        // MARKET -> OUTSIDE
        // ==========================================
        if (insideMarket)
        {
            player.position = outsideDoor;
            insideMarket = false;

            renderer.textures.SetCurrentPartition(
                TextureManager::Partition::Street
            );
        }

        // ==========================================
        // NEON -> OUTSIDE
        // ==========================================
        else if (insideNeon)
        {
            player.position = outsideNeonDoor;
            insideNeon = false;

            renderer.textures.SetCurrentPartition(
                TextureManager::Partition::Street
            );
        }

        // ==========================================
        // OUTSIDE -> ENTER SOMETHING
        // ==========================================
        else
        {
            float marketDist =
                Vector2Distance(
                    player.position,
                    outsideDoor
                );

            float neonDist =
                Vector2Distance(
                    player.position,
                    outsideNeonDoor
                );

            // ------------------------------
            // ENTER NEON
            // ------------------------------
            if (neonDist < marketDist)
            {
                player.position = insideNeonDoor;
                insideNeon = true;
                insideMarket = false;

                renderer.textures.SetCurrentPartition(
                    TextureManager::Partition::NeonNight
                );
            }

            // ------------------------------
            // ENTER MARKET
            // ------------------------------
            else
            {
                player.position = insideDoor;
                insideMarket = true;
                insideNeon = false;

                renderer.textures.SetCurrentPartition(
                    TextureManager::Partition::Market
                );
            }
        }

        renderer.StartFadeOut();
    }

    renderer.DrawFade();

   // DrawFPS(20, 20);

    EndDrawing();

}

void Game::Shutdown()
{
    // ============================================================
    // UNLOAD NPCs
    // ============================================================

    for (NPC& npc : npcs)
    {
        npc.Unload();
    }

    npcs.clear();

    // ============================================================
    // CLOSE AUDIO
    // ============================================================

    if (IsAudioDeviceReady())
    {
        CloseAudioDevice();
    }

    // ============================================================
    // CLOSE WINDOW
    // ============================================================

    CloseWindow();
}

void Game::Run()
{
    Initialize();

    while (running)
    {
        Update();

        Draw();
    }
    Shutdown();
}