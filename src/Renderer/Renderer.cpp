#include "Renderer.h"

#include <cmath>

Renderer::Renderer()
{
}

void Renderer::DrawSky()
{
    DrawRectangle(
        0,
        0,
        Config::SCREEN_WIDTH,
        Config::SCREEN_HEIGHT / 2,
        SKYBLUE);
}

void Renderer::DrawFloor()
{
    DrawRectangle(
        0,
        Config::SCREEN_HEIGHT / 2,
        Config::SCREEN_WIDTH,
        Config::SCREEN_HEIGHT / 2,
        DARKGRAY);
}

float Renderer::CastSingleRay(
    Vector2 playerPos,
    Vector2 rayDir,
    Map& map,
    int& side)
{
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

        sideDistX =
            (playerPos.x - mapX) *
            deltaDistX;
    }
    else
    {
        stepX = 1;

        sideDistX =
            (mapX + 1.0f - playerPos.x) *
            deltaDistX;
    }

    if (rayDir.y < 0)
    {
        stepY = -1;

        sideDistY =
            (playerPos.y - mapY) *
            deltaDistY;
    }
    else
    {
        stepY = 1;

        sideDistY =
            (mapY + 1.0f - playerPos.y) *
            deltaDistY;
    }

    bool hit = false;

    while (!hit)
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

        if (map.GetCell(mapY, mapX) == 1)
            hit = true;
    }

    float perpWallDist;

    if (side == 0)
        perpWallDist =
        sideDistX - deltaDistX;
    else
        perpWallDist =
        sideDistY - deltaDistY;

    return perpWallDist;
}
void Renderer::DrawWallColumn(
    int screenX,
    float distance,
    int side)
{
    if (distance < 0.1f)
        distance = 0.1f;

    int lineHeight =
        (int)(Config::SCREEN_HEIGHT / distance);

    int drawStart =
        Config::SCREEN_HEIGHT / 2 -
        lineHeight / 2;

    int drawEnd =
        Config::SCREEN_HEIGHT / 2 +
        lineHeight / 2;

    if (drawStart < 0)
        drawStart = 0;

    if (drawEnd >= Config::SCREEN_HEIGHT)
        drawEnd = Config::SCREEN_HEIGHT - 1;

    Color color =
        (side == 0)
        ? RED
        : MAROON;

    DrawRectangle(
        screenX,
        drawStart,
        1,
        drawEnd - drawStart,
        color);
}

void Renderer::Draw(
    Vector2 playerPos,
    Vector2 playerDir,
    Vector2 cameraPlane,
    Map& map)
{
    DrawSky();

    DrawFloor();

    for (int x = 0; x < Config::SCREEN_WIDTH; x++)
    {
        float cameraX =
            2.0f * x /
            (float)Config::SCREEN_WIDTH - 1.0f;

        Vector2 rayDir =
        {
            playerDir.x + cameraPlane.x * cameraX,
            playerDir.y + cameraPlane.y * cameraX
        };

        int side = 0;

        float distance =
            CastSingleRay(
                playerPos,
                rayDir,
                map,
                side);

        DrawWallColumn(
            x,
            distance,
            side);
    }
}