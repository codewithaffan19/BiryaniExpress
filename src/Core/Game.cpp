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
void Game::ResetGame() {
    SetPartitionAndSound(
        TextureManager::Partition::Street
    );
    SetTargetFPS(Config::TARGET_FPS);


    //story 
    story.Initialize();

    //Game
    gameStarted = true;
    running = true;
    storyRestrictionWarning = false;
    insideMarket = false;
    insideNeon = false;
    teleportPending = false;
    gameOverCursorSet = false;
    outsideDoor = { 8.5f, 2.5f };
    insideDoor = { 11.5f, 2.5f };
    outsideNeonDoor = { 22.5f, 6.5f };
    insideNeonDoor = { 22.5f, 10.5f };
    //cutscene
    cutscene.playing = false;
    cutscene.finished = false;
    cutscene.transitioning = false;
    cutscene.currentScene = 0;
    cutscene.transitionTimer = 0.0f;
    cutscene.transitionDuration = 1.0f;
    cutscene.printedWords = 0;
    cutscene.wordTimer = 0.0f;
    cutscene.wordDelay = 0.08f;


    //player
    player.position = { 2.0f,2.0f };
    player.GetDirection() = { 1.0f,0.0f };
    player.GetCameraPlane() = { 0.0f,0.66f };
    player.moveSpeed = 3.0f;
    player.radius = 0.25f;
    player.currentWeaponIndex = 0;
    player.hitmessagetimer = 0;
    player.health = 85;
    player.currentTex = player.handTex;
    player.WeaponPouch[0].id = ITEM_BIRYANI;
    player.WeaponPouch[1].id = ITEM_EMPTY;
    player.WeaponPouch[2].id = ITEM_EMPTY;
    //Enemy
    Police.position = { 4.5f,19.5f };
    Police.totalframes = 3;
    Police.moveSpeed = 0.8f;
    Police.currentAttackTimer = 0.5f;
    Police.attackDamage = 6;
    Police.spriteSheet = LoadTexture("assets/textures/Police.png");
    for (auto& e : enemies) {
        e.health = 100;
        e.KnockBackTimer = 0.0f;
        e.hasPopped = false;
    }
    enemies[0].position = { 15.5f,12.5f };
    enemies[1].position = { 20.5f,23.5f };
    enemies[2].position = { 10.5f,10.5f };
    enemies[3].position = { 1.0f, 7.0f };
    enemies[4].position = { 13.0f, 14.0f };
    enemies[5].position = { 23.5f, 6.5f };
    enemies[6].position = { 21.5f,23.5f };
    //Menu 
    menu.backgroundLoaded = false;
    menu.settingsOpen = false;
    menu.pauseOpen = false;
    menu.brightness = 1.0f;
    menu.musicVolume = 1.0f;



}

// ================================================================
// PARTITION + AMBIENCE
// ================================================================

void Game::SetPartitionAndSound(TextureManager::Partition partition)
{
    renderer.textures.SetCurrentPartition(partition);

    switch (partition)
    {
    case TextureManager::Partition::Street:
        soundManager.SetAmbience(SoundManager::Ambience::Street);
        break;

    case TextureManager::Partition::Market:
        soundManager.SetAmbience(SoundManager::Ambience::Market);
        break;

    case TextureManager::Partition::NeonNight:
        soundManager.SetAmbience(SoundManager::Ambience::Neon);
        break;

    default:
        break;
    }
}

void Game::Initialize()
{
    InitWindow(
        Config::SCREEN_WIDTH,
        Config::SCREEN_HEIGHT,
        "Last Key"
    );

    // ============================================================
    // FULLSCREEN
    //
    // Resize the window to the monitor's native resolution BEFORE
    // toggling fullscreen â€” raylib's ToggleFullscreen() uses the
    // CURRENT window size as the fullscreen resolution, so doing
    // this first is what makes it fill the real screen instead of
    // a small 1280x720 video mode.
    //
    // Everything the game actually draws still targets a fixed
    // Config::SCREEN_WIDTH x SCREEN_HEIGHT surface internally (see
    // renderer.gameView) and gets scaled up to fit here â€” so this
    // is purely a display-mode change; nothing about how the game
    // is rendered depends on the real window size.
    // ============================================================

    int monitor = GetCurrentMonitor();
    int monitorWidth = GetMonitorWidth(monitor);
    int monitorHeight = GetMonitorHeight(monitor);

    if (monitorWidth > 0 && monitorHeight > 0)
    {
        SetWindowSize(monitorWidth, monitorHeight);
        ToggleFullscreen();
    }

    // ============================================================
// MENU
// ============================================================

    if (!menu.Initialize(
        "assets/menu/background.png"
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
        "assets/cutscene/image1.png",
        "assets/cutscene/image2.png",
        "assets/cutscene/image3.png",
        "assets/cutscene/image4.png",
        "assets/cutscene/image4.png",

        "assets/audio/cutsceneBg.ogg",
        "assets/audio/typing.wav",

        "assets/audio/cutsceneDialogue1.wav",
        "assets/audio/cutsceneDialogue2.wav",
        "assets/audio/cutsceneDialogue3.wav",
        "assets/audio/cutsceneDialogue4.wav",
        "assets/audio/cutsceneDialogue4.wav"
    ))
    {
        TraceLog(
            LOG_ERROR,
            "Failed to initialize cutscene!"
        );
    }
    if (!neonCutscene.Initialize(
        "assets/cutscene/neonimage1.png",
        "assets/cutscene/neonimage2.png",
        "assets/cutscene/neonimage3.png",
        "assets/cutscene/neonimage4.png",
        "assets/cutscene/neonimage5.png",

        "assets/audio/neonCutsceneBg.ogg",
        "assets/audio/typing.wav",

        "assets/audio/neonDialogue1.wav",
        "assets/audio/neonDialogue2.wav",
        "assets/audio/neonDialogue3.wav",
        "assets/audio/neonDialogue4.wav",
        "assets/audio/neonDialogue5.wav"
    ))
    {
        TraceLog(
            LOG_ERROR,
            "Failed to initialize neon cutscene!"
        );
    }
    winTexture =
        LoadTexture(
            "assets/textures/win.png"
        );
    renderer.LoadTextures();
    renderer.textures.SetCurrentPartition(
        TextureManager::Partition::Street
    );

    // ============================================================
    // AMBIENCE SOUND SLOTS
    //
    // Menu / Street / Market / Neon. Drop the matching files at
    // these paths (missing files are skipped safely, same as
    // every other asset in this project).
    // ============================================================

    soundManager.LoadSounds(
        "assets/audio/menuAmbience.ogg",
        "assets/audio/streetAmbience.ogg",
        "assets/audio/marketAmbience.ogg",
        "assets/audio/neonAmbience.ogg"
    );

    soundManager.SetVolume(menu.GetMusicVolume());

    // Game boots straight into the main menu.
    soundManager.SetAmbience(SoundManager::Ambience::Menu);

    // ============================================================
    // SFX SLOTS: FOOTSTEP + HIT
    // ============================================================

    soundManager.LoadSfx(
        "assets/audio/footstep.wav",
        "assets/audio/hit.wav"
    );

    soundManager.SetSfxVolume(menu.GetMusicVolume());

    EnableCursor();
    SetTargetFPS(Config::TARGET_FPS);

    // Hand
    Image hand = LoadImage("assets/textures/HandHand.png");
    ImageColorReplace(&hand, MAGENTA, BLANK);

    player.handTex = LoadTextureFromImage(hand);
    player.Weapon1Tex = LoadTexture("assets/textures/ChaklaHAND.png");
    player.MikeHandTex = LoadTexture("assets/textures/MikeHand.png");
    player.currentTex = player.handTex;
    UnloadImage(hand);

    // ==========================================
    // ENEMIES
    // ==========================================

    enemies.clear();

    Enemy BurgerBoy;
    BurgerBoy.spriteSheet =
        LoadTexture("assets/textures/BurgerBoy.png");
    BurgerBoy.totalframes = 4;
    BurgerBoy.moveSpeed = 0.8f;
    BurgerBoy.position = { 13.0f, 14.0f };
    BurgerBoy.currentAttackTimer = 0.2f;
    BurgerBoy.attackDamage = 5;

    Enemy AngryUncle;
    AngryUncle.spriteSheet = LoadTexture("assets/textures/uncle2.png");
    AngryUncle.position = { 10.5f,10.5f };
    AngryUncle.totalframes = 5;
    AngryUncle.Respawn = true;
    AngryUncle.RespawnTimer = 10.0f;
    AngryUncle.moveSpeed = 0.7f;
    AngryUncle.currentAttackTimer = 0.2f;
    AngryUncle.attackDamage = 10;

    Enemy AngryUncle2;
    AngryUncle2.spriteSheet = LoadTexture("assets/textures/uncle2.png");
    AngryUncle2.position = { 23.5f, 6.5f };
    AngryUncle2.totalframes = 5;
    AngryUncle2.moveSpeed = 0.7f;
    AngryUncle2.currentAttackTimer = 0.5f;
    AngryUncle2.attackDamage = 7;

    Enemy Thief;
    Thief.spriteSheet = LoadTexture("assets/textures/Chor.png");
    Thief.position = { 20.5f,23.5f };
    Thief.totalframes = 2;
    Thief.Respawn = true;
    Thief.RespawnTimer = 10.0f;
    Thief.moveSpeed = 0.9f;
    Thief.currentAttackTimer = 0.7f;
    Thief.attackDamage = 5;
    //Y
    Enemy Thief2;
    Thief2.spriteSheet = LoadTexture("assets/textures/Chor2.png");
    Thief2.position = { 1.0f, 7.0f };
    Thief2.totalframes = 2;
    Thief2.moveSpeed = 0.9f;
    Thief2.currentAttackTimer = 0.5f;
    Thief2.attackDamage = 3;

    Enemy Cow;
    Cow.spriteSheet = LoadTexture("assets/textures/UncleCow.png");
    Cow.position = { 15.5f,12.5f };
    Cow.Respawn = true;
    Cow.RespawnTimer = 7.0f;
    Cow.moveSpeed = 0.8f;
    Cow.totalframes = 3;
    Cow.currentAttackTimer = 0.6f;
    Cow.attackDamage = 10;

    Enemy Cow1;
    Cow1.spriteSheet = LoadTexture("assets/textures/UncleCow.png");
    Cow1.position = { 21.5f,23.5f };
    Cow1.Respawn = true;
    Cow1.RespawnTimer = 7.0f;
    Cow1.moveSpeed = 0.8f;
    Cow1.totalframes = 3;
    Cow1.currentAttackTimer = 0.6f;
    Cow1.attackDamage = 10;

    Police.position = { 4.5f,19.5f };
    Police.totalframes = 3;
    Police.moveSpeed = 0.8f;
    Police.currentAttackTimer = 0.5f;
    Police.attackDamage = 6;
    Police.spriteSheet = LoadTexture("assets/textures/Police.png");

    enemies.push_back(Cow);
    enemies.push_back(Thief);
    enemies.push_back(AngryUncle);
    enemies.push_back(Thief2);
    enemies.push_back(BurgerBoy);
    enemies.push_back(AngryUncle2);
    enemies.push_back(Cow1);



    // ==========================================
    // MAP
    // ==========================================

    map.LoadMap("assets/maps/test.txt");

    // ==========================================
    // NPCs
    // ==========================================

    npcs.clear();

    NPC afc;
    afc.myItem = ITEM_AFC;
    if (!afc.Load(
        "AFC Waiter",
        "assets/textures/AFC_waiter.png",
        { 13.5f, 1.5f }))
    {
        TraceLog(LOG_ERROR, "Failed to load AFC Waiter");
    }
    npcs.push_back(afc);

    NPC mike;
    mike.myItem = ITEM_MIKE;
    if (!mike.Load(
        "MIKE Waiter",
        "assets/textures/MIKE_waiter.png",
        { 13.5f, 3.5f }))
    {
        TraceLog(LOG_ERROR, "Failed to load MIKE Waiter");
    }
    npcs.push_back(mike);

    NPC clinex;
    clinex.myItem = ITEM_CLINIX;
    if (!clinex.Load(
        "CLINEX Waiter",
        "assets/textures/CLINEX_waiter.png",
        { 19.5f, 3.5f }))
    {
        TraceLog(LOG_ERROR, "Failed to load CLINEX Waiter");
    }
    npcs.push_back(clinex);

    NPC drumble;
    drumble.myItem = ITEM_DRUMBLE;
    if (!drumble.Load(
        "DRUMBLE Waiter",
        "assets/textures/DRUMBLE_waiter.png",
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
        "assets/textures/jack.png",
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

                soundManager.PlayHit();

                float knockBackForce = 4.0f;
                E.KnockBackVelocity = { normX * knockBackForce,normY * knockBackForce };
                E.KnockBackTimer = 0.2f;
                E.position = KnockBack(E.position, E.KnockBackVelocity, E.KnockBackTimer, E.radius, map, dt);
            }
        }
    }
}
void Game::Update()
{
    float dt = GetFrameTime();

    // Keep the ambience music stream fed every frame, regardless of
    // which screen/state is active (menu, cutscene, gameplay, pause,
    // game over, etc.), and pick up live volume-slider changes.
    soundManager.Update();
    soundManager.SetVolume(menu.GetMusicVolume());
    soundManager.SetSfxVolume(menu.GetMusicVolume());

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

            // Full reset (player, enemies, NPCs, doors, story) â€”
            // previously this only re-ran story.Initialize(), so
            // starting a new playthrough after a win kept stale
            // player/enemy/mission state from the finished run.
            ResetGame();
            gameStarted = false;

            soundManager.SetAmbience(SoundManager::Ambience::Menu);

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

            SetPartitionAndSound(
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

                // Gameplay starts on the Street partition â€”
                // switch ambience from Menu to Street to match.
                soundManager.SetAmbience(SoundManager::Ambience::Street);

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
            // Stop the menu ambience so it doesn't overlap the
            // cutscene's own background music.
            soundManager.Stop();

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
    // GAME OVER
    // ========================================================

    if (player.health <= 0)
    {
        // Only re-center/enable the cursor ONCE when the Game Over
        // screen first appears. Calling EnableCursor() every frame
        // (as before) re-centers the OS cursor every frame too, which
        // made it look "stuck" and unusable on the restart buttons.
        if (!gameOverCursorSet)
        {
            EnableCursor();
            gameOverCursorSet = true;
        }

        return;
    }


    // ========================================================
    // GAME
    // ========================================================
    input.Update();
    storyRestrictionWarning = false;

    // ==========================================
    // GAME UPDATE
    // ==========================================

    if (!story.IsPlayerLocked())
    {
        player.Update(dt, input, map);

        // Footstep sound â€” fires on an interval while the player is
        // actually moving (WASD held); silent otherwise.
        if (player.IsMoving())
        {
            footstepTimer += dt;

            if (footstepTimer >= 0.35f)
            {
                footstepTimer = 0.0f;
                soundManager.PlayFootstep();
            }
        }
        else
        {
            footstepTimer = 0.0f;
        }

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
            player.PickUpItem(ITEM_SPOON);
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

                    renderer.StartFadeIn(Renderer::LoadingTransition::Market);
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

                        renderer.StartFadeIn(Renderer::LoadingTransition::Neon);
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
                    renderer.StartFadeIn(Renderer::LoadingTransition::Market);
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

    if (input.IsKeyPressed(KEY_ESCAPE))
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
        BeginTextureMode(renderer.gameView);
        ClearBackground(BLACK);

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

        EndTextureMode();
        renderer.DrawGameViewToScreen();

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
    if (player.health <= 0) {
        // NOTE: cursor is enabled once (edge-triggered) in Update(),
        // not here every frame â€” see gameOverCursorSet. Calling
        // EnableCursor() every frame re-centers the OS cursor every
        // frame, which is what made it look stuck on this screen.
        BeginTextureMode(renderer.gameView);
        ClearBackground(BLACK);

        renderer.DrawGameOver();

        EndTextureMode();
        renderer.DrawGameViewToScreen();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse = Renderer::GetVirtualMousePosition();
            Renderer::GameOverButtons rects = renderer.GetGameOverButtonRects();

            if (CheckCollisionPointRec(mouse, rects.retry)) {
                ResetGame();
                DisableCursor();
            }
            else if (CheckCollisionPointRec(mouse, rects.mainMenu)) {
                ResetGame();
                gameStarted = false;
                soundManager.SetAmbience(SoundManager::Ambience::Menu);
                EnableCursor();
            }
            else if (CheckCollisionPointRec(mouse, rects.exit)) {
                running = false;
            }
        }

        EndDrawing();
        return;
    }
    if (menu.IsPauseOpen())
    {
        menu.DrawPause();

        EndDrawing();
        return;
    }

    // ========================================================
    // GAME
    //
    // Rendered into a fixed Config::SCREEN_WIDTH x SCREEN_HEIGHT
    // texture (renderer.gameView), then scaled up to fill the
    // real fullscreen window afterward â€” see
    // Renderer::DrawGameViewToScreen(). Everything in this block
    // keeps using Config::SCREEN_WIDTH/HEIGHT exactly as before.
    // ========================================================
    BeginTextureMode(renderer.gameView);
    ClearBackground(RAYWHITE);
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

            SetPartitionAndSound(
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
                // Stop the street ambience so it doesn't overlap
                // the neon cutscene's own background music.
                soundManager.Stop();

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

                SetPartitionAndSound(
                    TextureManager::Partition::Market
                );
            }
        }

        renderer.StartFadeOut();
    }

    renderer.DrawFade();

    EndTextureMode();

    // Scale the fixed-resolution frame up to fill the real window.
    renderer.DrawGameViewToScreen();

    //DrawFPS(20, 20);

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
    // UNLOAD AMBIENCE SOUND
    // ============================================================

    soundManager.Unload();
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