#pragma once

#include "../Player/Player.h"
#include "../World/Map.h"
#include "../Renderer/Renderer.h"
#include "../Core/InputManager.h"
#include "../Editor/Editor.h"
#include "../Enemy.h"
#include <vector>
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

    bool running;

    Player player;

    Map map;

    Renderer renderer;

    InputManager input;
    Editor editor;
    std::vector<Enemy>enemies;
    bool editorMode = false;
};