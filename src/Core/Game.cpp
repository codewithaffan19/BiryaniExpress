#include "Game.h"
#include "raylib.h"
#include "Config.h"
#include "InputManager.h"
#include <cmath>
#include "../Renderer/Renderer.h"
#include "../World/Map.h"
#include "../Renderer/TextureManager.h"
#include<iostream>
#include "raymath.h"
#include "../World/Collision.h"
#include "../Cutscene/Cutscene.h"
static bool HasAFCAndDrumble(const Player& player)
{
    return
        player.MissionCount[0] >= player.MissionTarget[0] &&
        player.MissionCount[1] >= player.MissionTarget[1];
}
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
// MENU
// ============================================================

    if (!menu.Initialize(
        "../assets/menu/background.png"
    ))
    {
        TraceLog(
            LOG_ERROR,
            "Failed to initialize menu!"
        );
    }

    gameStarted = false;

    EnableCursor();
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
    // ============================================================
// CUTSCENE
// ============================================================

    if (!cutscene.Initialize(
        "../assets/cutscene/image1.png",
        "../assets/cutscene/image2.png",
        "../assets/cutscene/image3.png",
        "../assets/cutscene/image4.png",
        "../assets/cutscene/image4.png",

        "../assets/cutscene/audio1.wav",
        "../assets/cutscene/audio2.wav",
        "../assets/cutscene/audio3.wav",
        "../assets/cutscene/audio4.wav",
        "../assets/cutscene/audio4.wav"
    ))
    {
        TraceLog(
            LOG_ERROR,
            "Failed to initialize cutscene!"
        );
    }
    if (!neonCutscene.Initialize(
        "../assets/cutscene/neonimage1.png",
        "../assets/cutscene/neonimage2.png",
        "../assets/cutscene/neonimage3.png",
        "../assets/cutscene/neonimage4.png",
        "../assets/cutscene/neonimage5.png",

        "../assets/cutscene/neonaudio1.wav",
        "../assets/cutscene/neonaudio2.wav",
        "../assets/cutscene/neonaudio3.wav",
        "../assets/cutscene/neonaudio4.wav",
        "../assets/cutscene/neonaudio5.wav"
    ))
    {
        TraceLog(
            LOG_ERROR,
            "Failed to initialize neon cutscene!"
        );
    }
    winTexture =
        LoadTexture(
            "../assets/textures/win.png"
        );
    renderer.LoadTextures();
    renderer.textures.SetCurrentPartition(
        TextureManager::Partition::Street
    );
    editor.SetTextureManager(&renderer.textures);

    EnableCursor();
    SetTargetFPS(Config::TARGET_FPS);

    // Hand
    Image hand = LoadImage("../assets/textures/hand.png");
    ImageColorReplace(&hand, MAGENTA, BLANK);

    player.handTex = LoadTextureFromImage(hand);
    player.Weapon1Tex=LoadTexture("../../assets/textures/Weapon1.png");
    player.MikeHandTex = LoadTexture("../../assets/textures/mikeHand3.png");
    player.currentTex = player.handTex; 
    UnloadImage(hand);

    // ==========================================
    // ENEMIES
    // ==========================================

    enemies.clear();

    Enemy BurgerBoy;
    BurgerBoy.spriteSheet =
        LoadTexture("../../assets/textures/BurgerBoy.png");
    BurgerBoy.totalframes = 4;
    BurgerBoy.moveSpeed = 0.8f;
    BurgerBoy.position = { 13.0f, 14.0f };
    BurgerBoy.currentAttackTimer = 0.2f;
    BurgerBoy.attackDamage = 5;

    Enemy AngryUncle;
    AngryUncle.spriteSheet = LoadTexture("../../assets/textures/uncle2.png");
    AngryUncle.position = { 10.5f,10.5f };
    AngryUncle.totalframes = 5;
    AngryUncle.Respawn = true;
    AngryUncle.RespawnTimer = 10.0f;
    AngryUncle.moveSpeed = 0.7f;
    AngryUncle.currentAttackTimer = 0.2f;
    AngryUncle.attackDamage = 10;

    Enemy AngryUncle2;
    AngryUncle2.spriteSheet =LoadTexture("../../assets/textures/uncle2.png");
    AngryUncle2.position = { 23.5f, 6.5f };
    AngryUncle2.totalframes = 5;
    AngryUncle2.moveSpeed = 0.7f;
    AngryUncle2.currentAttackTimer = 0.5f;
    AngryUncle2.attackDamage = 7;

    Enemy Thief;
    Thief.spriteSheet =LoadTexture("../../assets/textures/Chor.png");
    Thief.position = { 20.5f,23.5f };
    Thief.totalframes = 2;
    Thief.Respawn = true;
    Thief.RespawnTimer = 10.0f;
    Thief.moveSpeed = 0.9f;
    Thief.currentAttackTimer = 0.7f;
    Thief.attackDamage = 5;
    //Y
    Enemy Thief2;
    Thief2.spriteSheet =LoadTexture("../../assets/textures/Chor2.png");
    Thief2.position = { 1.0f, 7.0f };
    Thief2.totalframes = 2;
    Thief2.moveSpeed = 0.9f;
    Thief2.currentAttackTimer=0.5f;
    Thief2.attackDamage = 8;

    Enemy Cow;
    Cow.spriteSheet = LoadTexture("../../assets/textures/UncleCow.png");
    Cow.position = { 15.5f,12.5f };
    Cow.Respawn = true;
    Cow.RespawnTimer = 7.0f;
    Cow.moveSpeed = 0.8f;
    Cow.totalframes = 3;
    Cow.currentAttackTimer = 0.6f;
    Cow.attackDamage = 10;

    Enemy Cow1;
    Cow1.spriteSheet = LoadTexture("../../assets/textures/UncleCow.png");
    Cow1.position = { 21.5f,23.5f };
    Cow1.Respawn = true;
    Cow1.RespawnTimer = 7.0f;
    Cow1.moveSpeed = 0.8f;
    Cow1.totalframes = 3;
    Cow1.currentAttackTimer = 0.6f;
    Cow1.attackDamage = 10;

    Police.position = {4.5f,19.5f};
    Police.totalframes = 3;
    Police.moveSpeed = 0.8f;
    Police.currentAttackTimer = 0.5f;
    Police.attackDamage = 6;
    Police.spriteSheet = LoadTexture("../../assets/textures/Police.png");

    enemies.push_back(Cow);
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
    // ==========================================
// STORY
// ==========================================

    story.Initialize();

}
void Game::CheckSpoonCollosion(Player& p, Enemy& E)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        float dt = GetFrameTime();
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

                float knockBackForce = 4.0f;
                E.KnockBackVelocity = { normX * knockBackForce,normY * knockBackForce };
                E.KnockBackTimer = 0.2f;
                E.position=KnockBack(E.position, E.KnockBackVelocity, E.KnockBackTimer, E.radius, map, dt);
            }
        }
    }
}
void Game::Update()
{
    float dt = GetFrameTime();
    if (winScreenActive)
    {
        if (winZoom < 0.7f)
        {
            winZoom += dt;
        }

        if (IsKeyPressed(KEY_ENTER))
        {
            winScreenActive = false;

            gameStarted = false;

            winZoom = 0.5f;

            story.Initialize();

            EnableCursor();
        }

        return;
    }
    if (neonCutsceneActive)
    {
        neonCutscene.Update();

        if (neonCutscene.IsFinished())
        {
            neonCutsceneActive = false;

            player.position = insideNeonDoor;

            insideNeon = true;
            insideMarket = false;

            renderer.textures.SetCurrentPartition(
                TextureManager::Partition::NeonNight
            );
        }

        return;
    }
    // ========================================================
    // MAIN MENU
    // ========================================================

    if (!gameStarted)
    {
        // ====================================================
        // CUTSCENE
        // ====================================================

        if (cutscene.IsPlaying())
        {
            cutscene.Update();

            // Cutscene finished -> start actual game
            if (cutscene.IsFinished())
            {
                gameStarted = true;

                DisableCursor();

                TraceLog(
                    LOG_INFO,
                    "Cutscene finished. Starting game..."
                );
            }

            return;
        }


        Menu::Action action =
            menu.Update();


        if (action == Menu::Action::Play)
        {
            cutscene.Start();

            EnableCursor();

            TraceLog(
                LOG_INFO,
                "Starting cutscene..."
            );
        }
        else if (action == Menu::Action::Exit)
        {
            running = false;
        }

        return;
    }


    // ========================================================
    // PAUSE MENU
    // ========================================================

    if (menu.IsPauseOpen())
    {
        Menu::Action action =
            menu.UpdatePause();

        if (action == Menu::Action::Exit)
        {
            running = false;
        }

        return;
    }



    // ========================================================
    // GAME
    // ========================================================
    input.Update();
    storyRestrictionWarning = false;
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
        if (!story.IsPlayerLocked())
        {
            player.Update(dt, input, map);
            for (int i = 0; i < enemies.size(); i++) {
                enemies[i].update(player, map);
            }
        }

        for (int i = 0; i < enemies.size(); i++)
        {
          

            CheckPlayerEnemyCollision(
                player,
                enemies[i],
                map
            );
            CheckSpoonCollosion(
                player,
                enemies[i]
            );
            CheckEnemyEnemyCollision(enemies, map);
        }

        for (int i = 0; i < npcs.size(); i++)
        {
            CheckPlayerNPCCollision(
                player,
                npcs[i],
                map
            );
        }
        story.CheckPlayerGuardCollision(
            player,
            map
        );
        if (player.isPoliceTriggered) {
            enemies.push_back(Police);
            player.isPoliceTriggered = false;
        }
    }
    // ==========================================
    // UPDATE NPCs
    // ==========================================

    Vector2 playerPos = player.GetPosition();
    story.Update(
        player,
        insideNeon
    );
    if (story.IsWon())
    {
        winScreenActive = true;
    }
    // ============================================================
    // STORY DOORS
    // ============================================================

    if (!editorMode)
    {
        // ========================================================
        // INSIDE NEON
        //
        // Neon is one-way.
        // Player cannot leave Neon through the entrance.
        // ========================================================

        if (insideNeon)
        {
            // Intentionally empty.
            // Story controls Neon exit.
        }

        // ========================================================
        // INSIDE MARKET
        //
        // Player MUST collect:
        // Drumble = MissionCount[0]
        // AFC     = MissionCount[1]
        //
        // before leaving Market.
        // ========================================================

        else if (insideMarket)
        {
            float dist =
                Vector2Distance(
                    player.GetPosition(),
                    insideDoor
                );

            if (dist < 1.2f && IsKeyPressed(KEY_E))
            {
                if (HasAFCAndDrumble(player))
                {
                    // ------------------------------------------
                    // MARKET EXIT ALLOWED
                    // ------------------------------------------

                    renderer.StartFadeIn();
                    teleportPending = true;
                }
                else
                {
                    // ------------------------------------------
                    // MARKET EXIT BLOCKED
                    // ------------------------------------------

                    storyRestrictionWarning = true;
                }
            }
        }

        // ========================================================
        // OUTSIDE / STREET
        //
        // Market can be entered normally.
        //
        // Neon requires AFC + Drumble first.
        // ========================================================

        else
        {
            float marketDist =
                Vector2Distance(
                    player.GetPosition(),
                    outsideDoor
                );

            float neonDist =
                Vector2Distance(
                    player.GetPosition(),
                    outsideNeonDoor
                );

            // ====================================================
            // PLAYER PRESSED E NEAR A DOOR
            // ====================================================

            if (IsKeyPressed(KEY_E))
            {
                // ----------------------------------------------
                // NEON DOOR
                // ----------------------------------------------

                if (neonDist < 1.2f)
                {
                    if (HasAFCAndDrumble(player))
                    {
                        // --------------------------------------
                        // NEON ENTRY ALLOWED
                        // --------------------------------------

                        renderer.StartFadeIn();
                        teleportPending = true;
                    }
                    else
                    {
                        // --------------------------------------
                        // NEON ENTRY BLOCKED
                        // --------------------------------------

                        storyRestrictionWarning = true;
                    }
                }

                // ----------------------------------------------
                // MARKET DOOR
                // ----------------------------------------------

                else if (marketDist < 1.2f)
                {
                    // Market entry is always allowed.
                    renderer.StartFadeIn();
                    teleportPending = true;
                }
            }
        }
    }
    for (NPC& npc : npcs)
    {
        npc.Update(playerPos, player);
    }


    // ==========================================
// ESCAPE / PAUSE
// ==========================================

    if (!editorMode &&
        input.IsKeyPressed(KEY_ESCAPE))
    {
        menu.OpenPause();

        return;
    }
}


void Game::Draw()
{
    BeginDrawing();

    ClearBackground(BLACK);
    if (winScreenActive)
    {
        float width =
            winTexture.width * winZoom;

        float height =
            winTexture.height * winZoom;

        DrawTexturePro(
            winTexture,
            {
                0,
                0,
                (float)winTexture.width,
                (float)winTexture.height
            },
        {
            Config::SCREEN_WIDTH / 2.0f,
            Config::SCREEN_HEIGHT / 2.0f,
            width,
            height
        },
        {
            width / 2.0f,
            height / 2.0f
        },
            0,
            WHITE
        );

        DrawText(
            "PRESS ENTER",
            Config::SCREEN_WIDTH / 2 - 120,
            Config::SCREEN_HEIGHT - 80,
            30,
            WHITE
        );

        EndDrawing();

        return;
    }
    // ====================================================
    // NEON CUTSCENE
    // ====================================================

    if (neonCutsceneActive)
    {
        neonCutscene.Draw();

        EndDrawing();

        return;
    }

    // ====================================================
    // MAIN MENU + OPENING CUTSCENE
    // ====================================================

    if (!gameStarted)
    {
        if (cutscene.IsPlaying())
        {
            cutscene.Draw();

            EndDrawing();

            return;
        }

        menu.Draw();

        EndDrawing();

        return;
    }


    // ========================================================
    // PAUSE MENU
    // ========================================================

    if (menu.IsPauseOpen())
    {
        menu.DrawPause();

        EndDrawing();
        return;
    }

    // ========================================================
    // GAME
    // ========================================================
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
            nearDoor,
            story,
            insideNeon
        );
        story.DrawUI();
        // ==========================================
// STORY RESTRICTION WARNING
// ==========================================

        if (storyRestrictionWarning)
        {
            const char* warningText;

            if (insideMarket)
            {
                warningText =
                    "Collect AFC and Drumble first.";
            }
            else
            {
                warningText =
                    "Go to Market and collect AFC and Drumble first.";
            }

            int fontSize = 28;

            int textWidth =
                MeasureText(warningText, fontSize);

            DrawText(
                warningText,
                Config::SCREEN_WIDTH / 2 - textWidth / 2,
                Config::SCREEN_HEIGHT - 130,
                fontSize,
                YELLOW
            );
        }
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

        if (insideMarket)
        {
            if (Vector2Distance(
                playerPos,
                insideDoor
            ) < 1.2f)
            {
                if (HasAFCAndDrumble(player))
                {
                    DrawText(
                        "Press E to Exit",
                        Config::SCREEN_WIDTH / 2 - 120,
                        Config::SCREEN_HEIGHT - 80,
                        24,
                        YELLOW
                    );
                }
                else
                {
                    DrawText(
                        "AFC + Drumble required",
                        Config::SCREEN_WIDTH / 2 - 145,
                        Config::SCREEN_HEIGHT - 80,
                        24,
                        YELLOW
                    );
                }
            }
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

            // ==========================================
            // MARKET DOOR
            // ==========================================

            if (marketDist < 1.2f)
            {
                DrawText(
                    "Press E to Enter Market",
                    Config::SCREEN_WIDTH / 2 - 140,
                    Config::SCREEN_HEIGHT - 80,
                    24,
                    YELLOW
                );
            }

            // ==========================================
            // NEON DOOR
            // ==========================================

            else if (neonDist < 1.2f)
            {
                if (HasAFCAndDrumble(player))
                {
                    DrawText(
                        "Press E to Enter Neon",
                        Config::SCREEN_WIDTH / 2 - 135,
                        Config::SCREEN_HEIGHT - 80,
                        24,
                        YELLOW
                    );
                }
                else
                {
                    DrawText(
                        "AFC + Drumble required",
                        Config::SCREEN_WIDTH / 2 - 145,
                        Config::SCREEN_HEIGHT - 80,
                        24,
                        YELLOW
                    );
                }
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

        // ========================================================
        // MARKET -> OUTSIDE
        // ========================================================

        if (insideMarket)
        {
            player.position = outsideDoor;
            insideMarket = false;

            renderer.textures.SetCurrentPartition(
                TextureManager::Partition::Street
            );
        }

        // ========================================================
        // OUTSIDE -> ENTER SOMETHING
        // ========================================================

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

            // ====================================================
            // ENTER NEON
            // ====================================================
            if (neonDist < marketDist)
            {
                neonCutscene.Start(true);

                neonCutsceneActive = true;
            }

            // ====================================================
            // ENTER MARKET
            // ====================================================

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

    DrawFPS(20, 20);

    EndDrawing();

}

void Game::Shutdown()
{
    UnloadTexture(winTexture);
    cutscene.Unload();
    neonCutscene.Unload();
    // ============================================================
    // UNLOAD NPCs
    // ============================================================

    for (NPC& npc : npcs)
    {
        npc.Unload();
    }

    npcs.clear();
    // ============================================================
// UNLOAD STORY
// ============================================================

    story.Unload();
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