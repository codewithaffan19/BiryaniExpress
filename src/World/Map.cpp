#include "Map.h"

#include <fstream>

Map::Map()
{
    for (int r = 0;r < ROWS;r++)
    {
        for (int c = 0;c < COLS;c++)
        {
            if (r == 0 || c == 0 || r == ROWS - 1 || c == COLS - 1)
                grid[r][c] = 1;
            else
                grid[r][c] = 0;
        }
    }
}

int Map::GetCell(int row, int col) const
{
    if (row < 0 || col < 0 || row >= ROWS || col >= COLS)
        return 1;

    return grid[row][col];
}

void Map::SetCell(int row, int col, int value)
{
    if (row < 0 || col < 0 || row >= ROWS || col >= COLS)
        return;

    grid[row][col] = value;
}

void Map::LoadMap(const std::string& file)
{
    std::ifstream in(file);

    if (!in.is_open())
        return;

    for (int r = 0;r < ROWS;r++)
    {
        for (int c = 0;c < COLS;c++)
        {
            in >> grid[r][c];
        }
    }
}

void Map::DrawGrid()
{
    int tile = 40;

    for (int r = 0;r < ROWS;r++)
    {
        for (int c = 0;c < COLS;c++)
        {
            Color color =
                grid[r][c] == 0 ?
                RAYWHITE :
                DARKGRAY;

            DrawRectangle(
                c * tile,
                r * tile,
                tile,
                tile,
                color);

            DrawRectangleLines(
                c * tile,
                r * tile,
                tile,
                tile,
                BLACK);
        }
    }
}
int Map::GetRows() const
{
    return ROWS;
}

int Map::GetCols() const
{
    return COLS;
}
void Map::SaveMap(const std::string& file)
{
    std::ofstream out(file);

    if (!out.is_open())
    {
        TraceLog(LOG_ERROR, "Cannot open file!");
        return;
    }

    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            out << grid[r][c] << " ";
        }
        out << "\n";
    }

    out.flush();
    out.close();

    TraceLog(LOG_INFO, "Map Saved!");
}