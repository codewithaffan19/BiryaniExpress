#pragma once

#include <vector>

#include "../Player/Player.h"
#include "../World/Map.h"
#include "../Renderer/Renderer.h"
#include "../Core/InputManager.h"
#include "../Editor/Editor.h"
#include "../Enemy.h"
#include "../NPC/NPC.h"
#include "../Story/Story.h"
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
private:
    bool running = false;
    bool storyRestrictionWarning = false;
    Player player;
    Map map;

    Renderer renderer;

    InputManager input;
    Editor editor;

    std::vector<Enemy> enemies;
    std::vector<NPC> npcs;
    Story story;
    bool editorMode = false;
    bool insideMarket = false;
    bool insideNeon = false;
    bool teleportPending = false;

    Vector2 outsideDoor = { 8.5f, 2.5f };
    Vector2 insideDoor = { 11.5f, 2.5f };
    Vector2 outsideNeonDoor = { 22.5f, 6.5f };
    Vector2 insideNeonDoor = { 22.5f, 10.5f };
};