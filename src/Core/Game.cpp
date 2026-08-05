#include "Game.h"

#include "raylib.h"

#include "Config.h"

#include "InputManager.h"

#include <cmath>
#include "../Renderer/Renderer.h"
#include "../World/Map.h"
#include "../Renderer/TextureManager.h"
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

    DisableCursor();
    SetTargetFPS(Config::TARGET_FPS);

    Image Spoon = LoadImage("../assets/textures/spoon.png");
    ImageColorReplace(&Spoon, MAGENTA, BLANK);
    player.spoonTex = LoadTextureFromImage(Spoon);
    //Enemies 
    enemies.clear();
    //Burger Boy
    Enemy BurgerBoy;
    BurgerBoy.moveSpeed = 1.0f;
    BurgerBoy .position = { 5.5f,6.5f };
    //Uncle takla
    Enemy AngryUncle;
    AngryUncle.position = { 4.5f,5.5f };
    AngryUncle.moveSpeed = 0.7f;

    enemies.push_back(AngryUncle);
    enemies.push_back(BurgerBoy);

    UnloadImage(Spoon);
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