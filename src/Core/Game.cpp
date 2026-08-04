#include "Game.h"

#include "raylib.h"

#include "Config.h"

#include "InputManager.h"

#include <cmath>
#include "../Renderer/Renderer.h"
#include "../World/Map.h"
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

    DisableCursor();
    SetTargetFPS(Config::TARGET_FPS);

    enemies.clear();
    Enemy E1;
    E1.position = {5.5f,6.5f};
    enemies.push_back(E1);


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
            map, enemies
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