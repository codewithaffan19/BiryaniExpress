#pragma once

#include <vector>
#include <string>
#include "raylib.h"

struct RayHit
{
    float distance;
    int side;
    int tile;
    float wallX;
};

class Map
{
public:

    Map();

    int GetCell(int row, int col) const;

    void SetCell(int row, int col, int value);

    void SaveMap(const std::string& file);

    void LoadMap(const std::string& file);

    void DrawGrid();

    int GetRows() const;
    int GetCols() const;
    RayHit CastSingleRay(
        Vector2 playerPos,
        Vector2 rayDir,
        Map& map,
        int& side);
private:

    static const int ROWS = 25;
    static const int COLS = 25;

    int grid[ROWS][COLS];
};