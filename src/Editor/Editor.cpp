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

    const int sidebar = 150;

    int col = (int)((mouse.x - SIDEBAR_WIDTH - cameraOffset.x) / tileSize);
    int row = (int)((mouse.y - cameraOffset.y) / tileSize);

    //--------------------------------
    // Select tile
    //--------------------------------

    if (IsKeyPressed(KEY_ZERO))
        currentTile = 0;

    if (IsKeyPressed(KEY_ONE))
        currentTile = 1;

    //--------------------------------
    // Paint
    //--------------------------------

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
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

    DrawRectangle(
        0,
        0,
        150,
        GetScreenHeight(),
        LIGHTGRAY);

    DrawText(
        "EDITOR",
        20,
        20,
        25,
        BLACK);

    DrawRectangle(
        20,
        80,
        25,
        25,
        WHITE);

    DrawRectangleLines(
        20,
        80,
        25,
        25,
        BLACK);

    DrawText(
        "0 Empty",
        55,
        84,
        18,
        BLACK);

    DrawRectangle(
        20,
        120,
        25,
        25,
        RED);

    DrawRectangleLines(
        20,
        120,
        25,
        25,
        BLACK);

    DrawText(
        "1 Wall",
        55,
        124,
        18,
        BLACK);

    DrawText(
        TextFormat("Current : %d", currentTile),
        20,
        180,
        20,
        RED);

    DrawText(
        "S = Save",
        20,
        220,
        20,
        BLACK);

}

void Editor::SaveShortcut(Map& map)
{
    if (IsKeyPressed(KEY_S))
    {
        map.SaveMap("../assets/maps/test.txt");
    }
}

