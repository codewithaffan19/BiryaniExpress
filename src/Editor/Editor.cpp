#include "Editor.h"
#include "../World/Map.h"
#include<fstream>
const int SIDEBAR_WIDTH = 220;
Editor::Editor()
{
}

void Editor::Update(Map& map)
{
    Vector2 mouse = GetMousePosition();

    int y = 55;

    for (int i = 0; i < 20; i++)
    {
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
            currentTile = i;
        }

        y += 32;
    }
    // -------------------------
// Mouse Wheel Zoom
// -------------------------

    float wheel = GetMouseWheelMove();

    if (wheel != 0)
    {
        tileSize += wheel * 4.0f;

        if (tileSize < 16)
            tileSize = 16;

        if (tileSize > 120)
            tileSize = 120;
    }

    // -------------------------
// Ctrl + / Ctrl - Zoom
// -------------------------

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

    const int sidebar = 150;

    int col = (int)((mouse.x - SIDEBAR_WIDTH - cameraOffset.x) / tileSize);
    int row = (int)((mouse.y - cameraOffset.y) / tileSize);



    //--------------------------------
    // Paint
    //--------------------------------

    if (mouse.x > SIDEBAR_WIDTH &&
        IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        map.SetCell(row, col, currentTile);
    }

    //--------------------------------
    // Camera Pan
    //--------------------------------

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

            switch (tile)
            {
            case 0: color = WHITE; break;
            case 1: color = DARKGRAY; break;
            case 2: color = BROWN; break;
            case 3: color = ORANGE; break;
            case 4: color = PINK; break;
            case 5: color = PURPLE; break;
            case 6: color = BLUE; break;
            case 7: color = GREEN; break;
            case 8: color = YELLOW; break;
            case 9: color = RED; break;
            }

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

    DrawRectangle(0, 0, SIDEBAR_WIDTH, GetScreenHeight(), LIGHTGRAY);

    DrawText("EDITOR", 20, 15, 24, BLACK);

    int y = 55;

    for (int i = 0; i < 20; i++)
    {
        Rectangle item =
        {
            10,
            (float)y,
            SIDEBAR_WIDTH - 20,
            28
        };

        if (i == currentTile)
            DrawRectangleRec(item, SKYBLUE);

        DrawRectangleLinesEx(item, 1, BLACK);

        DrawText(
            TextFormat("[%d] %s",
                i,
                textures->tileNames[i].c_str()),
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

