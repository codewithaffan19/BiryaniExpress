#include "Map.h"

#include <fstream>
#include<cmath>
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

RayHit Map::CastSingleRay(
    Vector2 playerPos,
    Vector2 rayDir,
    Map& map,
    int& side)
{
    RayHit hit;

    int mapX = (int)playerPos.x;
    int mapY = (int)playerPos.y;

    float deltaDistX =
        (rayDir.x == 0.0f)
        ? 1e30f
        : fabsf(1.0f / rayDir.x);

    float deltaDistY =
        (rayDir.y == 0.0f)
        ? 1e30f
        : fabsf(1.0f / rayDir.y);

    int stepX;
    int stepY;

    float sideDistX;
    float sideDistY;

    if (rayDir.x < 0)
    {
        stepX = -1;
        sideDistX = (playerPos.x - mapX) * deltaDistX;
    }
    else
    {
        stepX = 1;
        sideDistX = (mapX + 1.0f - playerPos.x) * deltaDistX;
    }

    if (rayDir.y < 0)
    {
        stepY = -1;
        sideDistY = (playerPos.y - mapY) * deltaDistY;
    }
    else
    {
        stepY = 1;
        sideDistY = (mapY + 1.0f - playerPos.y) * deltaDistY;
    }

    bool found = false;

    while (!found)
    {
        if (sideDistX < sideDistY)
        {
            sideDistX += deltaDistX;
            mapX += stepX;
            side = 0;
        }
        else
        {
            sideDistY += deltaDistY;
            mapY += stepY;
            side = 1;
        }

        if (map.GetCell(mapY, mapX) != 0)
            found = true;
    }

    float perpWallDist;

    if (side == 0)
        perpWallDist = sideDistX - deltaDistX;
    else
        perpWallDist = sideDistY - deltaDistY;

    float wallX;

    if (side == 0)
        wallX = playerPos.y + perpWallDist * rayDir.y;
    else
        wallX = playerPos.x + perpWallDist * rayDir.x;

    wallX -= floorf(wallX);

    hit.distance = perpWallDist;
    hit.side = side;
    hit.tile = map.GetCell(mapY, mapX);
    hit.wallX = wallX;

    return hit;
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

    out.close();

    TraceLog(LOG_INFO, "Map Saved!");
}
static bool IsSolidTile(int tile)
{
    return tile >= 1 && tile <= 30;
}

