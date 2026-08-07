#include "Game.h"

#include "raylib.h"

#include "Config.h"

#include "InputManager.h"

#include <cmath>
#include "../Renderer/Renderer.h"
#include "../World/Map.h"
#include "../Renderer/TextureManager.h"
#include "raymath.h"
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
    renderer.LoadTextures();
    editor.SetTextureManager(&renderer.textures);
    DisableCursor();
    SetTargetFPS(Config::TARGET_FPS);

    Image hand = LoadImage("../assets/textures/hand.png");
    ImageColorReplace(&hand, MAGENTA, BLANK);

    player.handTex = LoadTextureFromImage(hand);

    UnloadImage(hand);
    //Enemies 
    enemies.clear();
    //Burger Boy
    Enemy BurgerBoy;
    BurgerBoy.moveSpeed = 1.0f;
    BurgerBoy .position = { 5.5f,6.5f };
    //Uncle takla
    Enemy AngryUncle;
    AngryUncle.spriteSheet = LoadTexture("../assets/textures/uncle.png");
    AngryUncle.position = { 4.5f,5.5f };
    AngryUncle.moveSpeed = 0.7f;

    enemies.push_back(AngryUncle);
    enemies.push_back(BurgerBoy);

    map.LoadMap("../assets/maps/test.txt");
}

void Game::Update()
{
    float dt = GetFrameTime();

    input.Update();

    if (IsKeyPressed(KEY_TAB))
    {
        editorMode = !editorMode;

        if (editorMode)
            EnableCursor();
        else
            DisableCursor();
    }

    if (editorMode)
    {
        editor.Update(map);
    }
    else
    {
        player.Update(dt, input, map);
        for (int i = 0; i < enemies.size(); i++) {
            enemies[i].update(player, map);
        }

    }
    Vector2 playerPos = player.GetPosition();

    Vector2 targetDoor =
        insideMarket ? insideDoor : outsideDoor;

    float dist =
        Vector2Distance(playerPos, targetDoor);

    if (dist < 1.2f)
    {
        if (IsKeyPressed(KEY_E))
        {
            renderer.StartFadeIn();
            teleportPending = true;
        }
    }
    if (input.IsKeyPressed(KEY_ESCAPE))
        running = false;
}

void Game::Draw()
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    if (editorMode)
    {
        editor.Draw(map);
    }
    else
    {
        renderer.Draw(
            player.position,
            player.GetDirection(),
            player.GetCameraPlane(),
            map, enemies,player
        );
    }
    Vector2 playerPos = player.GetPosition();

    Vector2 targetDoor =
        insideMarket ? insideDoor : outsideDoor;

    float distOutside =
        Vector2Distance(playerPos, outsideDoor);

    float distInside =
        Vector2Distance(playerPos, insideDoor);

    if (distOutside < 1.2f || distInside < 1.2f)
    {
        DrawText(
            insideMarket ?
            "Press E to Exit"
            :
            "Press E to Enter",

            Config::SCREEN_WIDTH / 2 - 120,
            Config::SCREEN_HEIGHT - 80,
            24,
            YELLOW
        );
    }
    renderer.UpdateFade(GetFrameTime());
    if (teleportPending &&
        renderer.IsFadeFinished())
    {
        teleportPending = false;

        if (!insideMarket)
        {
            player.position = insideDoor;
        }
        else
        {
            player.position = outsideDoor;
        }

        insideMarket = !insideMarket;

        // start fade out
        renderer.StartFadeOut();
    }
    renderer.DrawFade();

    DrawFPS(20, 20);

    EndDrawing();
}


void Game::Shutdown()
{
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