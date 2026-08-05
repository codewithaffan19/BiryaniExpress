#include "Renderer.h"
#include "../World/Map.h"
#include <cmath>

Renderer::Renderer()
{
    wallTexture = { 0 };
}

void Renderer::LoadTextures()
{
    wallTexture = LoadTexture("../assets/textures/wall1.png");
}
void Renderer::UnloadTextures()
{

    UnloadTexture(wallTexture);
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


void Renderer::DrawWallColumn(
    int screenX,
    float distance,
    int side,
    int tile,
    float wallX)
{
    if (distance < 0.1f)
        distance = 0.1f;

    int lineHeight = (int)(Config::SCREEN_HEIGHT / distance);

    int drawStart = Config::SCREEN_HEIGHT / 2 - lineHeight / 2;
    int drawEnd = Config::SCREEN_HEIGHT / 2 + lineHeight / 2;

    if (drawStart < 0) drawStart = 0;
    if (drawEnd >= Config::SCREEN_HEIGHT)
        drawEnd = Config::SCREEN_HEIGHT - 1;

    // OLD RED WALL
    if (tile == 1)
    {
        Color color = (side == 0) ? RED : MAROON;

        DrawRectangle(
            screenX,
            drawStart,
            1,
            drawEnd - drawStart,
            color);

        return;
    }

    // NEW TEXTURED WALL
    if (tile == 2)
    {
        int texX = (int)(wallX * wallTexture.width);

        if (side == 0)
            texX = wallTexture.width - texX - 1;

        if (texX < 0)
            texX = 0;

        if (texX >= wallTexture.width)
            texX = wallTexture.width - 1;

        Rectangle source =
        {
            (float)texX,
            0.0f,
            1.0f,
            (float)wallTexture.height
        };

        Rectangle dest =
        {
            (float)screenX,
            (float)drawStart,
            1.0f,
            (float)(drawEnd - drawStart)
        };

        DrawTexturePro(
            wallTexture,
            source,
            dest,
            { 0,0 },
            0.0f,
            WHITE
        );

        return;
    }
}
void Renderer::Draw(
    Vector2 playerPos,
    Vector2 playerDir,
    Vector2 cameraPlane,
    Map& map,
    const std::vector<Enemy>& enemies,
    Player player) 
{
    // INITIALIZE THE Z-BUFFER
    // This array will hold the distance of the wall for every pixel column
    float Zbuffer[Config::SCREEN_WIDTH];

    DrawSky();
    DrawFloor();

    // ==========================================
    // PHASE 1: DRAW WALLS & LOG DISTANCES
    // ==========================================
    for (int x = 0; x < Config::SCREEN_WIDTH; x++)
    {
        float cameraX = 2.0f * x / (float)Config::SCREEN_WIDTH - 1.0f;

        Vector2 rayDir =
        {
            playerDir.x + cameraPlane.x * cameraX,
            playerDir.y + cameraPlane.y * cameraX
        };

        int side = 0;

        RayHit hit =
            map.CastSingleRay(
                playerPos,
                rayDir,
                map,
                side);

        DrawWallColumn(
            x,
            hit.distance,
            hit.side,
            hit.tile,
            hit.wallX);

        // 3. LOG THE WALL DISTANCE INTO THE Z-BUFFER
        Zbuffer[x] = hit.distance;
    }

    // PHASE 2: DRAW ENEMIES
    for (size_t i = 0; i < enemies.size(); i++)
    {
        // Calculate sprite position relative to the player
        Vector2 sprite = { enemies[i].position.x - playerPos.x, enemies[i].position.y - playerPos.y };

        // Camera Matrix Math (Translates 2D coordinates into 3D camera depth)
        float invDet = 1.0f / (cameraPlane.x * playerDir.y - cameraPlane.y * playerDir.x);
        float transformX = invDet * (playerDir.y * sprite.x - playerDir.x * sprite.y);
        float transformY = invDet * (-cameraPlane.y * sprite.x + cameraPlane.x * sprite.y); // transformY is the depth!

        // Only process the enemy if they are IN FRONT of the camera
        if (transformY > 0)
        {
            int spriteScreenX = int((Config::SCREEN_WIDTH / 2) * (1 + (transformX / transformY)));
            int spriteHeight = abs(int(Config::SCREEN_HEIGHT / transformY));
            int spriteWidth = spriteHeight; // Assuming square sprites

            // Calculate vertical drawing boundaries
            int drawStartY = -(spriteHeight / 2) + (Config::SCREEN_HEIGHT / 2);
            if (drawStartY < 0) drawStartY = 0;

            int drawEndY = (spriteHeight / 2) + (Config::SCREEN_HEIGHT / 2);
            if (drawEndY >= Config::SCREEN_HEIGHT) drawEndY = Config::SCREEN_HEIGHT - 1;

            // Calculate horizontal drawing boundaries
            int drawStartX = -(spriteWidth / 2) + spriteScreenX;
            if (drawStartX < 0) drawStartX = 0;

            int drawEndX = (spriteWidth / 2) + spriteScreenX;
            if (drawEndX >= Config::SCREEN_WIDTH) drawEndX = Config::SCREEN_WIDTH - 1;

            float texWidth = 64.0f; // Width of your enemy .png
            float texHeight = 64.0f; // Height of your enemy .png

            // Draw the enemy vertical stripe by vertical stripe
            for (int stripe = drawStartX; stripe < drawEndX; stripe++)
            {
                // 4. THE Z-BUFFER CHECK
                // Only draw this vertical slice IF it is closer than the wall (Zbuffer[stripe])
                if (stripe > 0 && stripe < Config::SCREEN_WIDTH && transformY < Zbuffer[stripe])
                {
                    int trueStartX = -(spriteWidth / 2) + spriteScreenX;
                    int texX = int((stripe - trueStartX) * texWidth / spriteWidth);

                    // Clamp to prevent pulling pixels outside the image
                    if (texX < 0) texX = 0;
                    if (texX >= texWidth) texX = texWidth - 1;

                    Rectangle sourceRec = { (float)texX, 0.0f, 1.0f, texHeight };
                    Rectangle destRec = { (float)stripe, (float)drawStartY, 1.0f, (float)spriteHeight };
                    Vector2 origin = { 0.0f, 0.0f };

                    // Draw this specific enemy's texture
                    /*DrawTexturePro(enemies[i].texture, sourceRec, destRec, origin, 0.0f, WHITE);*/
                    DrawLine(stripe, drawStartY, stripe, drawEndY, GREEN);

                }
            }
        }
    }
    Texture2D weaponTex;
    if (player.currentWeaponIndex == WEAPON_SPOON)
        weaponTex=player.spoonTex;

    // ... previous size/scale calculations ...
    float scale = (Config::SCREEN_HEIGHT * 0.5f) / weaponTex.height;
    float drawWidth = weaponTex.width * scale;
    float drawHeight = weaponTex.height * scale;

    // --- NEW BOBBING MATH ---
    // The X-axis uses cosine to swing left and right (Amplitude: 30 pixels)
    float bobX = cos(player.weaponbobtimer) * 30.0f;

    // The Y-axis uses absolute sine to simulate the heavy "bounce" of footsteps (Amplitude: 20 pixels)
    // Using abs() ensures the weapon only bounces UP, never down through the floor
    float bobY = abs(sin(player.weaponbobtimer)) * 20.0f;

    // --- APPLY THE OFFSETS ---
    // Start with your base left-handed position (-20.0f), then add the bob
    float drawX = -20.0f + bobX;

    // Start with the bottom anchor, then push it DOWN based on the bounce
    float drawY = Config::SCREEN_HEIGHT - drawHeight + bobY;

    Rectangle sourceRec = { 0.0f, 0.0f, (float)weaponTex.width, (float)weaponTex.height };
    Rectangle destRec = { drawX, drawY, drawWidth, drawHeight };
    Vector2 origin = { 0.0f, 0.0f };

    DrawTexturePro(weaponTex, sourceRec, destRec, origin, 0.0f, WHITE);
}