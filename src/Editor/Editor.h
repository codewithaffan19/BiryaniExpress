#pragma once

#include "raylib.h"

class Map;

class Editor
{
public:
    Editor();

    void Update(Map& map);
    void Draw(Map& map);

private:

    Vector2 cameraOffset = { 0,0 };

    float tileSize = 40.0f;

    int currentTile = 1;

    void SaveShortcut(Map& map);
    static constexpr int SIDEBAR_WIDTH = 220;
    static constexpr int TILE_SIZE = 40;
};