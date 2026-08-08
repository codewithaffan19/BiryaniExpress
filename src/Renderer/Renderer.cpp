#include "Renderer.h"
#include "../World/Map.h"
#include <cmath>
#include "../Renderer/TextureManager.h"
//Enemy states
enum { STILL, CHASE, INTERROGATE, DEAD };
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

void Renderer::DrawSky(Vector2 playerDir)
{
    float angle = atan2f(playerDir.y, playerDir.x);

    float u =
        (angle / (2.0f * PI) + 0.5f) *
        textures.skyTex.width;

    Rectangle src =
    {
        u,
        0,
        (float)Config::SCREEN_WIDTH,
        (float)textures.skyTex.height
    };

    Rectangle dst =
    {
        0,
        0,
        (float)Config::SCREEN_WIDTH,
        (float)(Config::SCREEN_HEIGHT / 2)
    };

    DrawTexturePro(
        textures.skyTex,
        src,
        dst,
        { 0,0 },
        0,
        WHITE
    );

    if (src.x + src.width > textures.skyTex.width)
    {
        Rectangle src2 =
        {
            0,
            0,
            src.x + src.width - textures.skyTex.width,
            (float)textures.skyTex.height
        };

        Rectangle dst2 =
        {
            textures.skyTex.width - src.x,
            0,
            src2.width,
            (float)(Config::SCREEN_HEIGHT / 2)
        };

        DrawTexturePro(
            textures.skyTex,
            src2,
            dst2,
            { 0,0 },
            0,
            WHITE
        );
    }
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
    if (tile >= 2 && tile <= 21)
    {

        Texture2D* tex = &textures.tiles[tile];
        // Door uses idle/stop image depending on distance
        if (tile == 20 || tile == 21)
        {
            // Approximate player distance from wall
            if (distance < 1.5f)
            {
                if (textures.tileAnim[tile].id != 0)
                    tex = &textures.tileAnim[tile];
            }
        }
        // Only shops have animation
        if (tile >= 10 && tile<=19)
        {
            if (((int)(GetTime() * 2)) % 2 == 1)
            {
                if (textures.tileAnim[tile].id != 0)
                    tex = &textures.tileAnim[tile];
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
    Player player) {

    // INITIALIZE THE Z-BUFFER
    // This array will hold the distance of the wall for every pixel column

    float Zbuffer[Config::SCREEN_WIDTH];

    DrawSky(playerDir);
    DrawFloor(playerPos, playerDir, cameraPlane);

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
        if (enemies[i].state != DEAD) {
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
        // PHASE 3: DRAW THE 3D BUBBLES
        for (int j = 0; j < 12; j++) {
            if (enemies[i].bubbles[j].active) {

                // 1. Calculate bubble position relative to the player
                Vector2 bubbleSprite = { enemies[i].bubbles[j].position.x - playerPos.x, enemies[i].bubbles[j].position.y - playerPos.y };

                // 2. 3D Camera Projection Math (Same as the enemy body!)
                float invDet = 1.0f / (cameraPlane.x * playerDir.y - cameraPlane.y * playerDir.x);
                float transformX = invDet * (playerDir.y * bubbleSprite.x - playerDir.x * bubbleSprite.y);
                float transformY = invDet * (-cameraPlane.y * bubbleSprite.x + cameraPlane.x * bubbleSprite.y);

                // 3. Only draw if the bubble is in front of the camera
                if (transformY > 0) {
                    // Find exactly where on the screen the bubble should be
                    int bubbleScreenX = int((Config::SCREEN_WIDTH / 2) * (1 + (transformX / transformY)));

                    // Put them roughly in the middle of the screen height
                    int bubbleScreenY = Config::SCREEN_HEIGHT / 2;

                    // Scale the radius so bubbles get smaller the further away they are!
                    float projectedRadius = (enemies[i].bubbles[j].radius / transformY) * 2.0f;

                    unsigned char alpha = (unsigned char)(enemies[i].bubbles[j].life * 255);
                    Color bubbleColor = { 173, 216, 230, alpha };

                    DrawCircle(bubbleScreenX, bubbleScreenY, projectedRadius, bubbleColor);
                }
            }
        }
    }
    Texture2D& weaponTex = player.handTex;

    float scale = (Config::SCREEN_HEIGHT * 0.55f) / weaponTex.height;

    float drawWidth = weaponTex.width * scale;
    float drawHeight = weaponTex.height * scale;

    float bobX;
    float bobY;

    bool walking =
        IsKeyDown(KEY_W) ||
        IsKeyDown(KEY_A) ||
        IsKeyDown(KEY_S) ||
        IsKeyDown(KEY_D);

    if (walking)
    {
        bobX = cosf((float)GetTime() * 10.0f) * 18.0f;
        bobY = fabsf(sinf((float)GetTime() * 10.0f)) * 16.0f;
    }
    else
    {
        bobX = 0.0f;
        bobY = sinf((float)GetTime() * 2.0f) * 3.0f;
    }

    float drawX =
        Config::SCREEN_WIDTH / 2.0f
        - drawWidth / 2.0f
        + bobX;

    float drawY =
        Config::SCREEN_HEIGHT
        - drawHeight
        + 100.0f
        + bobY;

    Rectangle src =
    {
        0.0f,
        0.0f,
        (float)weaponTex.width,
        (float)weaponTex.height
    };

    Rectangle dst =
    {
        drawX,
        drawY,
        drawWidth,
        drawHeight
    };

    DrawTexturePro(
        weaponTex,
        src,
        dst,
        { 0,0 },
        0.0f,
        WHITE);
    DrawDoorMarker(
        playerPos,
        playerDir,
        cameraPlane,
        Zbuffer,
        { 9.5f, 2.5f });

    DrawDoorMarker(
        playerPos,
        playerDir,
        cameraPlane,
        Zbuffer,
        { 24.5f, 23.5f });
    if (player.hitmessagetimer > 0) {
        DrawText("BAM! UNCLE HIT!", GetScreenWidth() / 2 - 150, GetScreenHeight() / 2 + 100, 40, GREEN);
    }
}
void Renderer::DrawDoorMarker(
    Vector2 playerPos,
    Vector2 playerDir,
    Vector2 cameraPlane,
    float Zbuffer[],
    Vector2 markerPos)
{
    Vector2 sprite =
    {
        markerPos.x - playerPos.x,
        markerPos.y - playerPos.y
    };

    float invDet =
        1.0f /
        (cameraPlane.x * playerDir.y
            - cameraPlane.y * playerDir.x);

    float transformX =
        invDet *
        (playerDir.y * sprite.x
            - playerDir.x * sprite.y);

    float transformY =
        invDet *
        (-cameraPlane.y * sprite.x
            + cameraPlane.x * sprite.y);

    if (transformY <= 0)
        return;

    int screenX =
        (int)((Config::SCREEN_WIDTH / 2)
            * (1 + transformX / transformY));

    float bob =
        sinf(GetTime() * 3.0f) * 8.0f;

    int markerHeight =
        abs((int)(120 / transformY));

    int markerWidth = markerHeight;

    int drawY =
        Config::SCREEN_HEIGHT / 2
        - (int)(150 / transformY)
        + (int)bob;

    Color color =
        (((int)(GetTime() * 4)) % 2 == 0)
        ? YELLOW
        : GOLD;

    DrawTriangle(
        {
            (float)screenX,
            (float)drawY
        },
        {
            (float)(screenX - markerWidth / 2),
            (float)(drawY - markerHeight)
        },
        {
            (float)(screenX + markerWidth / 2),
            (float)(drawY - markerHeight)
        },
        color);
}
void Renderer::StartFadeIn()
{
    fadingIn = true;
    fadingOut = false;
}
bool Renderer::IsFadeFinished() const
{
    return fadeAlpha >= 255;
}
void Renderer::UpdateFade(float dt)
{
    if (fadingIn)
    {
        fadeAlpha += 350 * dt;

        if (fadeAlpha >= 255)
        {
            fadeAlpha = 255;
            fadingIn = false;
        }
    }

    if (fadingOut)
    {
        fadeAlpha -= 350 * dt;

        if (fadeAlpha <= 0)
        {
            fadeAlpha = 0;
            fadingOut = false;
        }
    }
}
void Renderer::DrawFade()
{
    if (fadeAlpha <= 0)
        return;

    DrawRectangle(
        0,
        0,
        Config::SCREEN_WIDTH,
        Config::SCREEN_HEIGHT,
        Fade(BLACK, fadeAlpha / 255.0f));
}
void Renderer::StartFadeOut()
{
    fadingOut = true;
    fadingIn = false;
}