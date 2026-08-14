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
#include "../Menu/Menu.h"
#include "../Cutscene/Cutscene.h"
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
    void ResetGame();
private:
    Music menuBGM{};
    Music streetBGM{};
    Music marketBGM{};
    Music neonBGM{};

    enum MusicState
    {
        MENU_MUSIC,
        STREET_MUSIC,
        MARKET_MUSIC,
        NEON_MUSIC
    };

    MusicState currentMusic = MENU_MUSIC;

    void ChangeMusic(MusicState newMusic);
    Menu menu;

    Cutscene cutscene;
    Cutscene neonCutscene;

    bool neonCutsceneActive = false;
    bool winScreenActive = false;

    Texture2D winTexture{};

    float winZoom = 0.5f;
    bool gameStarted = false;
    bool running = false;
    bool storyRestrictionWarning = false;
    Player player;
    Map map;

    Renderer renderer;

    InputManager input;
    Editor editor;
    Enemy Police;
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