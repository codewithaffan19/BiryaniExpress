#include "Editor.h"
#include "../World/Map.h"
#include "../Renderer/TextureManager.h"
#include<fstream>
static std::vector<int> GetPartitionTiles(
    TextureManager* textures,
    TextureManager::Partition partition)
{
    std::vector<int> result;

    if (!textures)
        return result;

    for (int i = 0; i < TextureManager::MAX_TILES; i++)
    {
        if (textures->IsTileInPartition(i, partition))
        {
            result.push_back(i);
        }
    }

    return result;
}
const int SIDEBAR_WIDTH = 220;
Editor::Editor()
{
}

void Editor::Update(Map& map)
{
    Vector2 mouse = GetMousePosition();

    //----------------------------------
    // Mouse Wheel
    //----------------------------------

    float wheel = GetMouseWheelMove();

    if (mouse.x < SIDEBAR_WIDTH)
    {
        // Sidebar scrolling

        if (wheel > 0)
            scrollOffset--;

        if (wheel < 0)
            scrollOffset++;

        if (scrollOffset < 0)
            scrollOffset = 0;

        std::vector<int> partitionTiles =
            GetPartitionTiles(textures, currentPartition);

        int maxScroll =
            (int)partitionTiles.size() - VISIBLE_ITEMS;

        if (maxScroll < 0)
            maxScroll = 0;

        if (scrollOffset > maxScroll)
            scrollOffset = maxScroll;
    }
    else
    {
        // Map zoom

        if (wheel != 0)
        {
            tileSize += wheel * 4.0f;

            if (tileSize < 16)
                tileSize = 16;

            if (tileSize > 120)
                tileSize = 120;
        }
    }

    //----------------------------------
    // CTRL + + / -
    //----------------------------------

    if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL))
    {
        if (IsKeyPressed(KEY_EQUAL) || IsKeyPressed(KEY_KP_ADD))
        {
            tileSize += 4;

            if (tileSize > 120)
                tileSize = 120;
        }

        if (IsKeyPressed(KEY_MINUS) || IsKeyPressed(KEY_KP_SUBTRACT))
        {
            tileSize -= 4;

            if (tileSize < 16)
                tileSize = 16;
        }
    }

    //----------------------------------
// Partition Tabs
//----------------------------------

    HandlePartitionSelection(mouse);

    //----------------------------------
    // Partition Tile Selection
    //----------------------------------

    std::vector<int> partitionTiles =
        GetPartitionTiles(textures, currentPartition);

    int y = 105;

    for (int i = 0; i < VISIBLE_ITEMS; i++)
    {
        int listIndex = i + scrollOffset;

        if (listIndex >= (int)partitionTiles.size())
            break;

        int tileIndex = partitionTiles[listIndex];

        Rectangle item =
        {
            10,
            (float)y,
            SIDEBAR_WIDTH - 20,
            28
        };

        if (CheckCollisionPointRec(mouse, item) &&
            IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            currentTile = tileIndex;
        }

        y += 32;
    }

    //----------------------------------
    // Map Cell
    //----------------------------------

    int col =
        (int)((mouse.x - SIDEBAR_WIDTH - cameraOffset.x) / tileSize);

    int row =
        (int)((mouse.y - cameraOffset.y) / tileSize);

    //----------------------------------
    // Paint
    //----------------------------------

    if (mouse.x > SIDEBAR_WIDTH &&
        IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        map.SetCell(row, col, currentTile);
    }

    //----------------------------------
    // Camera Pan
    //----------------------------------

    if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
    {
        Vector2 delta = GetMouseDelta();

        cameraOffset.x += delta.x;
        cameraOffset.y += delta.y;
    }

    SaveShortcut(map);
}
void Editor::Draw(Map& map)
{
    for (int r = 0; r < map.GetRows(); r++)
    {
        for (int c = 0; c < map.GetCols(); c++)
        {
            Rectangle rect =
            {
                SIDEBAR_WIDTH + cameraOffset.x + c * tileSize,
                cameraOffset.y + r * tileSize,
                tileSize,
                tileSize
            };

            int tile = map.GetCell(r, c);

            Color color = RAYWHITE;

            Color tileColors[31] =
            {
                WHITE,                          // 0
                DARKGRAY,                       // 1

                {139, 69, 19, 255},             // 2  Wall1
                {255, 140, 0, 255},             // 3  Wall2
                {255, 105, 180, 255},           // 4  Wall3
                {128, 0, 128, 255},             // 5  Wall4
                {30, 144, 255, 255},            // 6  Wall5
                {50, 205, 50, 255},             // 7  Wall6
                {255, 215, 0, 255},             // 8  Wall7
                {220, 20, 60, 255},             // 9  Wall8

                {0, 191, 255, 255},             // 10 Cafe
                {255, 165, 0, 255},             // 11 AFC
                {50, 255, 50, 255},              // 12 Drumble
                {255, 0, 255, 255},              // 13 CHIPS
                {210, 180, 140, 255},            // 14 Butcher

                {0, 0, 139, 255},                // 15 MarketWall1
                {0, 100, 0, 255},                // 16 MarketWall2
                {75, 0, 130, 255},               // 17 MarketWall3
                {128, 0, 0, 255},                // 18 AMW
                {0, 128, 128, 255},              // 19 SlimeLite

                {138, 43, 226, 255},              // 20 EntranceDoor
                {255, 20, 147, 255},              // 21 ExitDoor

                {0, 128, 255, 255},               // 22 Gym
                {255, 100, 0, 255},               // 23 Mike
                {255, 200, 0, 255},               // 24 KikoMilano
                {100, 255, 100, 255},             // 25 MehakPosh
                {0, 220, 220, 255},               // 26 CleanX

                {160, 82, 45, 255},               // 27 Bar
                {180, 70, 220, 255},              // 28 Brolex
                {40, 180, 100, 255},              // 29 DavidPutra
                {255, 50, 50, 255}                // 30 GamingZone
            };


            if (tile >= 0 && tile <= 30)
                color = tileColors[tile];
            else
                color = GRAY;

            DrawRectangleRec(rect, color);

            DrawRectangleLines(
                (int)rect.x,
                (int)rect.y,
                (int)rect.width,
                (int)rect.height,
                BLACK
            );
        }
    }

    DrawRectangle(
        0,
        0,
        SIDEBAR_WIDTH,
        GetScreenHeight(),
        LIGHTGRAY
    );

    DrawText("EDITOR", 20, 15, 24, BLACK);

    DrawPartitionTabs();


    std::vector<int> partitionTiles =
        GetPartitionTiles(textures, currentPartition);

    int y = 105;

    for (int i = 0; i < VISIBLE_ITEMS; i++)
    {
        int listIndex = i + scrollOffset;

        if (listIndex >= (int)partitionTiles.size())
            break;

        int tileIndex = partitionTiles[listIndex];
        Rectangle item =
        {
            10,
            (float)y,
            SIDEBAR_WIDTH - 20,
            28
        };

        if ( tileIndex== currentTile)
            DrawRectangleRec(item, SKYBLUE);

        DrawRectangleLinesEx(item, 1, BLACK);

        DrawText(
            TextFormat("[%d] %s",
                tileIndex,
                textures->tileNames[tileIndex].c_str()),
            18,
            y + 6,
            18,
            BLACK);

        y += 32;
    }

    DrawText(
        TextFormat("Current : %s",
            textures->tileNames[currentTile].c_str()),
        10,
        GetScreenHeight() - 60,
        20,
        RED);
    DrawText(
        TextFormat("Showing %d - %d",
            scrollOffset,
            scrollOffset + VISIBLE_ITEMS - 1),
        10,
        GetScreenHeight() - 90,
        18,
        DARKGRAY);
    DrawText(
        "S = Save",
        10,
        GetScreenHeight() - 30,
        18,
        BLACK);
}

void Editor::SaveShortcut(Map& map)
{
    if (IsKeyPressed(KEY_S))
    {
        map.SaveMap("../assets/maps/test.txt");
    }
}
void Editor::HandlePartitionSelection(Vector2 mouse)
{
    Rectangle streetButton =
    {
        10, 50,
        60, 32
    };

    Rectangle marketButton =
    {
        75, 50,
        60, 32
    };

    Rectangle nightButton =
    {
        140, 50,
        65, 32
    };

    if (CheckCollisionPointRec(mouse, streetButton) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        currentPartition = TextureManager::Partition::Street;
        scrollOffset = 0;
    }

    if (CheckCollisionPointRec(mouse, marketButton) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        currentPartition = TextureManager::Partition::Market;
        scrollOffset = 0;
    }

    if (CheckCollisionPointRec(mouse, nightButton) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        currentPartition = TextureManager::Partition::NeonNight;
        scrollOffset = 0;
    }
}
void Editor::DrawPartitionTabs()
{
    Rectangle streetButton =
    {
        10, 50,
        60, 32
    };

    Rectangle marketButton =
    {
        75, 50,
        60, 32
    };

    Rectangle nightButton =
    {
        140, 50,
        65, 32
    };

    Color streetColor = LIGHTGRAY;
    Color marketColor = LIGHTGRAY;
    Color nightColor = LIGHTGRAY;

    if (currentPartition == TextureManager::Partition::Street)
        streetColor = SKYBLUE;

    if (currentPartition == TextureManager::Partition::Market)
        marketColor = SKYBLUE;

    if (currentPartition == TextureManager::Partition::NeonNight)
        nightColor = SKYBLUE;

    DrawRectangleRec(streetButton, streetColor);
    DrawRectangleRec(marketButton, marketColor);
    DrawRectangleRec(nightButton, nightColor);

    DrawRectangleLinesEx(streetButton, 1, BLACK);
    DrawRectangleLinesEx(marketButton, 1, BLACK);
    DrawRectangleLinesEx(nightButton, 1, BLACK);

    DrawText("ST", 30, 59, 16, BLACK);
    DrawText("MK", 93, 59, 16, BLACK);
    DrawText("NN", 155, 59, 16, BLACK);
}

