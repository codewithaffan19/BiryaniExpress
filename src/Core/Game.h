#pragma once

#include <vector>

#include "../Player/Player.h"
#include "../World/Map.h"
#include "../Renderer/Renderer.h"
#include "../Core/InputManager.h"
#include "../Editor/Editor.h"
#include "../Enemy.h"
#include "../NPC/NPC.h"

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

private:
    bool running = false;

    Player player;
    Map map;

    Renderer renderer;

    InputManager input;
    Editor editor;

    std::vector<Enemy> enemies;
    std::vector<NPC> npcs;

    bool editorMode = false;
    bool insideMarket = false;

    bool teleportPending = false;

    Vector2 outsideDoor = { 8.5f, 2.5f };
    Vector2 insideDoor = { 11.5f, 2.5f };
};