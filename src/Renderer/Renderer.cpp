#include "Renderer.h"
#include "../World/Map.h"
#include <cmath>
#include "../Renderer/TextureManager.h"
Renderer::Renderer()
{
}


void Renderer::LoadTextures()
{
    textures.Load();
    int totalframes = Config::SCREEN_HEIGHT * Config::SCREEN_WIDTH;
    floorBuffer = new Color[totalframes];

    for (int i = 0; i < totalframes; i++) {
        floorBuffer[i] = BLANK;
    }
    Image blankImg = GenImageColor(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, BLANK);
    floorTexture = LoadTextureFromImage(blankImg);
    UnloadImage(blankImg);
}

void Renderer::UnloadTextures()
{
    textures.Unload();
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

void Renderer::DrawFloor(Vector2 playerPos, Vector2 playerDir, Vector2 cameraPlane)
{

    if (textures.floorimg.width <= 0 || textures.floorimg.height <= 0)
    {
        return;
    }
    // We loop through the bottom half of the screen (from the center horizon to the bottom edge)
    for (int y = Config::SCREEN_HEIGHT / 2 + 1; y < Config::SCREEN_HEIGHT; ++y)
    {
        // Calculate the ray directions for the leftmost (x=0) and rightmost (x=width) pixels of this specific row
        float rayDirX0 = playerDir.x - cameraPlane.x;
        float rayDirY0 = playerDir.y - cameraPlane.y;
        float rayDirX1 = playerDir.x + cameraPlane.x;
        float rayDirY1 = playerDir.y + cameraPlane.y;

        // 'p' is the current Y position compared to the center of the screen (the horizon)
        int p = y - Config::SCREEN_HEIGHT / 2;

        // Vertical position of the camera (simulating the height of the player's eyes)
        float posZ = 0.5f * Config::SCREEN_HEIGHT;

        // Horizontal distance from the camera to the floor for this specific row
        float rowDistance = posZ / p;

        // Calculate the "Step Vector" (How much we move in the 2D map for every 1 pixel we move right on the screen)
        float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / Config::SCREEN_WIDTH;
        float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / Config::SCREEN_WIDTH;

        // Real world coordinates of the leftmost pixel in this row. 
        // We will add the Step Vector to this as we loop across the screen.
        float floorX = playerPos.x + rowDistance * rayDirX0;
        float floorY = playerPos.y + rowDistance * rayDirY0;

        // Now, loop across every horizontal pixel in this specific row
        for (int x = 0; x < Config::SCREEN_WIDTH; ++x)
        {
            // Calculate the exact grid cell the floor coordinate is in
            int cellX = (int)(floorX);
            int cellY = (int)(floorY);

            // Get the exact texture coordinates based on the fractional part of the floor coordinates
            // Note: Replace 'textures.floorImg.width' with your actual width if you aren't using the TextureManager dynamically here
            int texWidth = textures.floorimg.width;
            int texHeight = textures.floorimg.height;

            // The bitwise AND (&) creates a perfect repeating/tiling effect!
            int tx = (int)(texWidth * (floorX - cellX)) & (texWidth - 1);
            int ty = (int)(texHeight * (floorY - cellY)) & (texHeight - 1);

            // Move our real-world position over by one Step Vector for the next pixel
            floorX += floorStepX;
            floorY += floorStepY;

            // 1. Get the exact pixel color from our CPU Image
            Color color = GetImageColor(textures.floorimg, tx, ty);

            int arrayIndex = y * Config::SCREEN_WIDTH + x;
            floorBuffer[arrayIndex] = color;
        }
    }
    UpdateTexture(floorTexture, floorBuffer);

    // Draw the entire floor to the screen in a single command!
    DrawTexture(floorTexture, 0, 0, WHITE);
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
    if (tile >= 2 && tile <= 9)
    {
        int texIndex = tile - 2;

        Texture2D* tex = &textures.walls[texIndex];

        // Animated walls (wall3 -> wall8)
        if (tile >= 4 && tile <= 9)
        {
            int animIndex = tile - 4;

            if (((int)(GetTime() * 2)) % 2 == 1)
            {
                if (textures.wallAnim[animIndex].id != 0)
                    tex = &textures.wallAnim[animIndex];
            }
        }

        if (tex->id == 0)
        {
            return;
        }
        int texX = (int)(wallX * tex->width);

        if (side == 0)
            texX = tex->width - texX - 1;

        if (texX < 0)
            texX = 0;

        if (texX >= tex->width)
            texX = tex->width - 1;

        Rectangle source =
        {
            (float)texX,
            0.0f,
            1.0f,
            (float)tex->height
        };

        Rectangle dest =
        {
            (float)screenX,
            (float)drawStart,
            1.0f,
            (float)(drawEnd - drawStart)
        };

        DrawTexturePro(
            *tex,
            source,
            dest,
            { 0,0 },
            0.0f,
            WHITE);

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
    DrawFloor(playerPos,playerDir,cameraPlane);

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

           
            float frameWidth = (float)enemies[i].spriteSheet.width / enemies[i].totalframes;
            float frameHeight = (float)enemies[i].spriteSheet.height;

            // 2. Calculate where this specific frame starts on the X axis of the image
            float frameOffsetX = enemies[i].currentframe * frameWidth;

            // Draw the enemy vertical stripe by vertical stripe
            for (int stripe = drawStartX; stripe < drawEndX; stripe++)
            {
                // 4. THE Z-BUFFER CHECK
                // Only draw this vertical slice IF it is closer than the wall (Zbuffer[stripe])
                if (stripe > 0 && stripe < Config::SCREEN_WIDTH && transformY < Zbuffer[stripe])
                {
                    int trueStartX = -(spriteWidth / 2) + spriteScreenX;

                    // Calculate which pixel of the current FRAME we are drawing
                    int texX = int((stripe - trueStartX) * frameWidth / spriteWidth);

                    // Clamp to prevent pulling pixels outside the frame bounds
                    if (texX < 0) texX = 0;
                    if (texX >= frameWidth) texX = frameWidth - 1;

                    // 3. Define the Source Rectangle (The 1-pixel wide slice of the image)
                    // Notice how we add `frameOffsetX` to `texX` to shift our window to the correct animation frame!
                    Rectangle sourceRec = { frameOffsetX + (float)texX, 0.0f, 1.0f, frameHeight };

                    Rectangle destRec = { (float)stripe, (float)drawStartY, 1.0f, (float)spriteHeight };
                    Vector2 origin = { 0.0f, 0.0f };

                    // 4. Draw the animated slice (Replacing the GREEN line)
                    DrawTexturePro(enemies[i].spriteSheet, sourceRec, destRec, origin, 0.0f, WHITE);
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