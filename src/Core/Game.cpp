#include "Game.h"
#include "raylib.h"
#include "Config.h"
#include "InputManager.h"
#include <cmath>
#include "../Renderer/Renderer.h"
#include "../World/Map.h"
#include "../Renderer/TextureManager.h"
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
    renderer.LoadTextures();
    editor.SetTextureManager(&renderer.textures);
    DisableCursor();
    SetTargetFPS(Config::TARGET_FPS);

    Image hand = LoadImage("../../assets/textures/hand.png");
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
    AngryUncle.spriteSheet = LoadTexture("../../assets/textures/uncle.png");
    AngryUncle.position = { 15.5f,2.5f };
    AngryUncle.moveSpeed = 0.7f;
    enemies.push_back(AngryUncle);
    enemies.push_back(BurgerBoy);
    map.LoadMap("../../assets/maps/test.txt");
}
void Game::CheckSpoonCollosion(Player& p, Enemy& E) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        float dx = E.position.x - p.position.x;
        float dy = E.position.y - p.position.y;
        float dis = sqrt((dx * dx) + (dy * dy));

        if (dis < 1.5) {
            float normX = dx / dis;
            float normY = dy / dis;

            float result = (p.GetDirection().x * normX) + (p.GetDirection().y * normY);
            if (result > 0.85f) {
                p.hitmessagetimer = 30;
            }
        }
    }
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
            CheckPlayerEnemyCollision(player, enemies[i]);
            CheckSpoonCollosion(player, enemies[i]);
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