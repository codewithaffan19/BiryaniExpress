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
            player.GetPosition(),
            player.GetDirection(),
            player.GetCameraPlane(),
            map
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