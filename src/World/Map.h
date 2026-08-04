#pragma once

#include <vector>
#include <string>
#include "raylib.h"

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
    float CastSingleRay(
        Vector2 playerPos,
        Vector2 rayDir,
        Map& map,
        int& side);
private:

    static const int ROWS = 20;
    static const int COLS = 20;

    int grid[ROWS][COLS];
};