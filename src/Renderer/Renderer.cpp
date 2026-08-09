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
void Renderer::DrawMarketRoof(Vector2 playerDir)
{
    float angle = atan2f(playerDir.y, playerDir.x);

    float u =
        (angle / (2.0f * PI) + 0.5f) *
        textures.marketRoofTex.width;

    Rectangle src =
    {
        u,
        0,
        (float)Config::SCREEN_WIDTH,
        (float)textures.marketRoofTex.height
    };

    Rectangle dst =
    {
        0,
        0,
        (float)Config::SCREEN_WIDTH,
        (float)(Config::SCREEN_HEIGHT / 2)
    };

    DrawTexturePro(
        textures.marketRoofTex,
        src,
        dst,
        { 0,0 },
        0,
        WHITE
    );

    if (src.x + src.width > textures.marketRoofTex.width)
    {
        Rectangle src2 =
        {
            0,
            0,
            src.x + src.width - textures.marketRoofTex.width,
            (float)textures.marketRoofTex.height
        };

        Rectangle dst2 =
        {
            textures.marketRoofTex.width - src.x,
            0,
            src2.width,
            (float)(Config::SCREEN_HEIGHT / 2)
        };

        DrawTexturePro(
            textures.marketRoofTex,
            src2,
            dst2,
            { 0,0 },
            0,
            WHITE
        );
    }
}

void Renderer::DrawHud(Player& p) {
    int barX = 20;
    int barY = 20;
    int MaxBarWidth = 320;
    int barHeight = 46;
    int bazel=4;
    float healthPercentage = (float) p.health / (float)p.maxhealth;
    if (healthPercentage < 0.0f) {
        healthPercentage = 0.0f;
    }
    int currentwidth = healthPercentage * MaxBarWidth;
    //Layer1 Dark outer shadow
    DrawRectangleRounded({ (float)barX - 2,(float)barY - 2,(float)MaxBarWidth + 4,(float)barHeight + 4 }, 0.15f, 4, BLACK);
   //Layer2 Metallic outer bazel
    DrawRectangleRounded({ (float)barX,(float)barY ,(float)MaxBarWidth,(float)barHeight }, 0.15f, 4, GRAY);
    //Layer3 DarkInnerTrough
    int innerX = barX + bazel;
    int innerY = barY + bazel;
    int innerW = MaxBarWidth - (bazel * 2);
    int innerH = barHeight - (bazel * 2);
    DrawRectangleRounded({ (float)innerX,(float)innerY,(float)innerW,(float)innerH }, 0.1f, 4, DARKGRAY);
    //Layer4 heart icon
    int IconBoxWidth = 42;
    DrawRectangle(innerX, innerY, IconBoxWidth, innerH, MAROON);
    DrawRectangleLines(innerX, innerY, IconBoxWidth, innerH, BLACK);
    //Layer5
    int BarX = innerX + IconBoxWidth + 4;
    int BarY = innerY + 4;
    int MaxBarW = innerW - IconBoxWidth - 8;
    int BarH = innerH - 8;
    int CurrentbarW = (int)(MaxBarW * healthPercentage);
    DrawRectangle(BarX, BarY, MaxBarW, BarH, BLACK);
    if (CurrentbarW > 0) {
        DrawRectangle(BarX, BarY, CurrentbarW, BarH, RED);

        // Optional Retro 3D Highlight Line (Top edge of red bar)
        DrawRectangle(BarX, BarY, CurrentbarW, 3, MAROON);
    }
    float size = 22;
    float r = size * 0.28f;
    float x = BarX - 35;
    float y = BarY + 5;
    // Two lobes (top of the heart)
    DrawCircleV({ x + r, y + r }, r, RED);
    DrawCircleV({ x + size - r, y + r }, r, RED);

    // Bottom point (triangle)
    Vector2 v1 = { x, y + r };
    Vector2 v2 = { x + size, y + r };
    Vector2 v3 = { x + size / 2, y + size };

    DrawTriangle(v1, v3, v2, RED);
    //Layer 6
    const char* healthStr = TextFormat("HEALTH %i%%", (int)(healthPercentage * 100));
    DrawText(healthStr, barX + 60, barY + 15, 20, WHITE);
}

void Renderer::DrawInventoryHUD(Player& p) {
    int slotSize = 70;
    int slotGap = 12;
    int slotX = Config::SCREEN_WIDTH - slotSize - 20;
    int startY = 20;

    for (int i = 0; i < 2; i++) {
        int slotY = startY + i * (slotSize + slotGap);
            bool isSelected = (i == p.currentWeaponIndex);
            Color BgColor = isSelected ? Fade(SKYBLUE, 0.4f) : Fade(BLACK, 0.4f);
            DrawRectangle(slotX, slotY, slotSize, slotSize, BgColor);

            Color BorderColor = isSelected ? YELLOW : GRAY;

            DrawRectangleLinesEx({ (float)slotX,(float)slotY,(float)slotSize,(float)slotSize }, isSelected ? 3.0f : 2.0f, BorderColor);
      
            DrawText(TextFormat("%d", i), slotX + 6, slotY + 6, 14, WHITE);
    }

}

void Renderer::DrawStreetFloor(Vector2 playerPos, Vector2 playerDir, Vector2 cameraPlane)
{
    // Row 2, columns 2-9
    if (row == 2 && col >= 2 && col <= 9)
        return true;

    // Rows 3-7, column 9
    if (col == 9 && row >= 3 && row <= 7)
        return true;

    // INDEX 6 -> DESIGN ROW 7
    if (row == 7 && col >= 2 && col <= 24)
        return true;

    return false;
}
static bool IsFloor2Cell(int row, int col)
{
    // Row 3, columns 2-8
    if (row == 3 && col >= 2 && col <= 8)
        return true;

    // Rows 4-6, column 8
    if (col == 8 && row >= 4 && row <= 6)
        return true;

    // INDEX 7 -> DESIGN ROW 8
    if (row == 8 && col >= 2 && col <= 24)
        return true;

    return false;
}
static bool IsFloorRotateLeftCell(int row, int col)
{
    // LEFT ROTATION
    // Index column 7, rows 3-5
    // Design column 8, rows 4-6

    if (col == 8 &&
        row >= 4 && row <= 6)
    {
        return true;
    }

    return false;
}

static bool IsFloorRotateRightCell(int row, int col)
{
    // RIGHT ROTATION
    // Index column 8, rows 2-5
    // Design column 9, rows 3-6

    if (col == 9 &&
        row >= 3 && row <= 6)
    {
        return true;
    }

    return false;
}
void Renderer::DrawWideFloor(
    Vector2 playerPos,
    Vector2 playerDir,
    Vector2 cameraPlane,
    Map& map)
{
    if (textures.floorimg.data == nullptr)
        return;

    if (textures.floor2Img.data == nullptr)
        return;

    // ==========================================
    // FLOOR RENDERING
    // ==========================================

    for (int y = Config::SCREEN_HEIGHT / 2 + 1;
        y < Config::SCREEN_HEIGHT;
        ++y)
    {
        float rayDirX0 =
            playerDir.x - cameraPlane.x;

        float rayDirY0 =
            playerDir.y - cameraPlane.y;

        float rayDirX1 =
            playerDir.x + cameraPlane.x;

        float rayDirY1 =
            playerDir.y + cameraPlane.y;

        int p =
            y - Config::SCREEN_HEIGHT / 2;

        float posZ =
            0.5f * Config::SCREEN_HEIGHT;

        float rowDistance =
            posZ / p;

        float floorStepX =
            rowDistance *
            (rayDirX1 - rayDirX0) /
            Config::SCREEN_WIDTH;

        float floorStepY =
            rowDistance *
            (rayDirY1 - rayDirY0) /
            Config::SCREEN_WIDTH;

        float floorX =
            playerPos.x +
            rowDistance * rayDirX0;

        float floorY =
            playerPos.y +
            rowDistance * rayDirY0;

        // ==========================================
        // DRAW EACH FLOOR PIXEL
        // ==========================================

        for (int x = 0;
            x < Config::SCREEN_WIDTH;
            ++x)
        {
            // ==========================================
            // WORLD CELL
            // ==========================================

            int cellX = (int)floorX;
            int cellY = (int)floorY;

            // ==========================================
            // COORDINATE CONVERSION
            //
            // Actual map/world coordinates are 0-based.
            // Your floor-design coordinates are 1-based.
            //
            // Example:
            //
            // World (0,0) -> Design (1,1)
            // World (1,1) -> Design (2,2)
            // ==========================================

            int designRow = cellY + 1;
            int designCol = cellX + 1;

            // ==========================================
            // DEFAULT FLOOR
            // ==========================================

            Color color = DARKGRAY;

            // ==========================================
// POSITION INSIDE CURRENT TILE
// ==========================================

            float u =
                floorX - cellX;

            float v =
                floorY - cellY;


            // ==========================================
            // ROTATED FLOOR CELLS
            // ==========================================
            //
            // Check this FIRST because some rotated
            // cells are not part of IsFloor1Cell().
            //
            // Pink area:
            // Rows 2-5
            // Columns 9-10
            // ==========================================

            // ==========================================
// ROTATED FLOOR CELLS
// ==========================================

            if (IsFloorRotateLeftCell(designRow, designCol))
            {
                int texWidth =
                    textures.floorimg.width;

                int texHeight =
                    textures.floorimg.height;

                // ==========================================
                // 90 DEGREE LEFT ROTATION
                // ==========================================

                int tx =
                    (int)(texWidth * (1.0f - v));

                int ty =
                    (int)(texHeight * u);

                // Safety
                if (tx < 0)
                    tx = 0;

                if (ty < 0)
                    ty = 0;

                if (tx >= texWidth)
                    tx = texWidth - 1;

                if (ty >= texHeight)
                    ty = texHeight - 1;

                color =
                    GetImageColor(
                        textures.floorimg,
                        tx,
                        ty
                    );
            }

            // ==========================================
            // ROTATE RIGHT
            // ==========================================

            else if (IsFloorRotateRightCell(designRow, designCol))
            {
                int texWidth =
                    textures.floorimg.width;

                int texHeight =
                    textures.floorimg.height;

                // ==========================================
                // 90 DEGREE RIGHT ROTATION
                // ==========================================

                int tx =
                    (int)(texWidth * v);

                int ty =
                    (int)(texHeight * (1.0f - u));

                // Safety
                if (tx < 0)
                    tx = 0;

                if (ty < 0)
                    ty = 0;

                if (tx >= texWidth)
                    tx = texWidth - 1;

                if (ty >= texHeight)
                    ty = texHeight - 1;

                color =
                    GetImageColor(
                        textures.floorimg,
                        tx,
                        ty
                    );
            }

            // ==========================================
            // NORMAL FLOOR 1
            // ==========================================

            else if (IsFloor1Cell(designRow, designCol))
            {
                int texWidth =
                    textures.floorimg.width;

                int texHeight =
                    textures.floorimg.height;

                int tx =
                    (int)(texWidth * u);

                int ty =
                    (int)(texHeight * v);

                // Safety
                if (tx < 0)
                    tx = 0;

                if (ty < 0)
                    ty = 0;

                if (tx >= texWidth)
                    tx = texWidth - 1;

                if (ty >= texHeight)
                    ty = texHeight - 1;

                color =
                    GetImageColor(
                        textures.floorimg,
                        tx,
                        ty
                    );
            }

            // ==========================================
            // NORMAL FLOOR 2
            // ==========================================

            else if (IsFloor2Cell(designRow, designCol))
            {
                int texWidth =
                    textures.floor2Img.width;

                int texHeight =
                    textures.floor2Img.height;

                int tx =
                    (int)(texWidth * u);

                int ty =
                    (int)(texHeight * v);

                // Safety
                if (tx < 0)
                    tx = 0;

                if (ty < 0)
                    ty = 0;

                if (tx >= texWidth)
                    tx = texWidth - 1;

                if (ty >= texHeight)
                    ty = texHeight - 1;

                color =
                    GetImageColor(
                        textures.floor2Img,
                        tx,
                        ty
                    );
            }

            // ==========================================
            // NO FLOOR
            // ==========================================

            else
            {
                color = DARKGRAY;
            }

            
            // ==========================================
            // NEXT WORLD POSITION
            // ==========================================

            floorX += floorStepX;
            floorY += floorStepY;

            // ==========================================
            // FLOOR BUFFER
            // ==========================================

            int arrayIndex =
                y * Config::SCREEN_WIDTH + x;

            floorBuffer[arrayIndex] =
                color;
        }
    }

    // ==========================================
    // UPLOAD FLOOR BUFFER TO GPU
    // ==========================================

    UpdateTexture(
        floorTexture,
        floorBuffer
    );

    // ==========================================
    // DRAW FLOOR
    // ==========================================

    DrawTexture(
        floorTexture,
        0,
        0,
        WHITE
    );
}
void Renderer::DrawMarketFloor(Vector2 playerPos, Vector2 playerDir, Vector2 cameraPlane)
{

    if (textures.marketFloorImg.width <= 0 || textures.marketFloorImg.height <= 0)
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
            int texWidth = textures.marketFloorImg.width;
            int texHeight = textures.marketFloorImg.height;

            // The bitwise AND (&) creates a perfect repeating/tiling effect!
            int tx = (int)(texWidth * (floorX - cellX)) & (texWidth - 1);
            int ty = (int)(texHeight * (floorY - cellY)) & (texHeight - 1);

            // Move our real-world position over by one Step Vector for the next pixel
            floorX += floorStepX;
            floorY += floorStepY;

            // 1. Get the exact pixel color from our CPU Image
            Color color = GetImageColor(textures.marketFloorImg, tx, ty);

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
    float wallX,
    float rayDirX,
    float rayDirY)
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
    if (tile >= 2 && tile <= 31)
    {
        Texture2D* tex = &textures.tiles[tile];

        // ==========================================
        // DOOR ANIMATION
        // ==========================================

        if (tile == 20 || tile == 21)
        {
            if (distance < 1.5f)
            {
                if (textures.tileAnim[tile].id != 0)
                    tex = &textures.tileAnim[tile];
            }
        }

        // ==========================================
        // SHOP ANIMATION
        // ==========================================

        if (
            (tile >= 2 && tile <= 14) ||
            tile == 22 ||
            tile == 23 ||
            tile == 24 ||
            tile == 25 ||
            tile == 26 ||
            tile == 27 ||
            tile == 28 ||
            tile == 29 ||
            tile == 30
            )
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

        if (side == 0 && rayDirX < 0)
            texX = tex->width - texX - 1;

        if (side == 1 && rayDirY > 0)
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
    const std::vector<NPC>& npcs,
    Player player,
    bool insideMarket)
{
    // ==========================================
    // Z-BUFFER
    // ==========================================

    float Zbuffer[Config::SCREEN_WIDTH];

    // ==========================================
    // SKY / ROOF + FLOOR
    // ==========================================

    if (insideMarket)
    {
        DrawMarketRoof(playerDir);

        DrawMarketFloor(
            playerPos,
            playerDir,
            cameraPlane
        );
    }
    else
    {
        DrawSky(playerDir);

        DrawWideFloor(
            playerPos,
            playerDir,
            cameraPlane,
            map
        );
    }

    // ==========================================
    // PHASE 1: WALLS
    // ==========================================

    for (int x = 0; x < Config::SCREEN_WIDTH; x++)
    {
        float cameraX =
            2.0f * x / (float)Config::SCREEN_WIDTH - 1.0f;

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
                side
            );

        DrawWallColumn(
            x,
            hit.distance,
            hit.side,
            hit.tile,
            hit.wallX,
            rayDir.x,
            rayDir.y
        );

        Zbuffer[x] = hit.distance;
    }

    // ==========================================
    // PHASE 2: ENEMIES
    // ==========================================

    for (size_t i = 0; i < enemies.size(); i++)
    {
        // Don't draw dead enemies
        if (enemies[i].state == DEAD)
            continue;

        // --------------------------------------
        // Position relative to player
        // --------------------------------------

        Vector2 sprite =
        {
            enemies[i].position.x - playerPos.x,
            enemies[i].position.y - playerPos.y
        };

        // --------------------------------------
        // Camera transformation
        // --------------------------------------

        float invDet =
            1.0f /
            (cameraPlane.x * playerDir.y -
                cameraPlane.y * playerDir.x);

        float transformX =
            invDet *
            (playerDir.y * sprite.x -
                playerDir.x * sprite.y);

        float transformY =
            invDet *
            (-cameraPlane.y * sprite.x +
                cameraPlane.x * sprite.y);

        // Enemy is behind player
        if (transformY <= 0.0f)
            continue;

        // --------------------------------------
        // Project enemy onto screen
        // --------------------------------------

        int spriteScreenX =
            (int)(
                (Config::SCREEN_WIDTH / 2.0f) *
                (1.0f + transformX / transformY)
                );

        int spriteHeight =
            abs(
                (int)(
                    Config::SCREEN_HEIGHT / transformY
                    )
            );

        int spriteWidth = spriteHeight;

        // --------------------------------------
        // Vertical boundaries
        // --------------------------------------

        int drawStartY =
            -spriteHeight / 2 +
            Config::SCREEN_HEIGHT / 2;

        int drawEndY =
            spriteHeight / 2 +
            Config::SCREEN_HEIGHT / 2;

        if (drawStartY < 0)
            drawStartY = 0;

        if (drawEndY >= Config::SCREEN_HEIGHT)
            drawEndY = Config::SCREEN_HEIGHT - 1;

        // --------------------------------------
        // Horizontal boundaries
        // --------------------------------------

        int drawStartX =
            -spriteWidth / 2 +
            spriteScreenX;

        int drawEndX =
            spriteWidth / 2 +
            spriteScreenX;

        if (drawStartX < 0)
            drawStartX = 0;

        if (drawEndX >= Config::SCREEN_WIDTH)
            drawEndX = Config::SCREEN_WIDTH - 1;

        // If completely outside screen
        if (drawStartX >= Config::SCREEN_WIDTH ||
            drawEndX < 0)
        {
            continue;
        }

        // --------------------------------------
        // Animation frame
        // --------------------------------------

        if (enemies[i].totalframes <= 0)
            continue;

        float frameWidth =
            (float)enemies[i].spriteSheet.width /
            enemies[i].totalframes;

        float frameHeight =
            (float)enemies[i].spriteSheet.height;

        float frameOffsetX =
            enemies[i].currentframe *
            frameWidth;

        // --------------------------------------
        // Draw enemy stripe-by-stripe
        // --------------------------------------

        for (int stripe = drawStartX;
            stripe < drawEndX;
            stripe++)
        {
            if (stripe < 0 ||
                stripe >= Config::SCREEN_WIDTH)
            {
                continue;
            }

            // Z-buffer test
            if (transformY >= Zbuffer[stripe])
                continue;

            int trueStartX =
                -spriteWidth / 2 +
                spriteScreenX;

            int texX =
                (int)(
                    (stripe - trueStartX) *
                    frameWidth /
                    spriteWidth
                    );

            // Clamp texture X
            if (texX < 0)
                texX = 0;

            if (texX >= (int)frameWidth)
                texX = (int)frameWidth - 1;

            // ----------------------------------
            // Source rectangle
            // ----------------------------------

            Rectangle sourceRec =
            {
                frameOffsetX + (float)texX,
                0.0f,
                1.0f,
                frameHeight
            };

            // ----------------------------------
            // Destination rectangle
            // ----------------------------------

            Rectangle destRec =
            {
                (float)stripe,
                (float)drawStartY,
                1.0f,
                (float)(drawEndY - drawStartY)
            };

            // ----------------------------------
            // Draw
            // ----------------------------------

            DrawTexturePro(
                enemies[i].spriteSheet,
                sourceRec,
                destRec,
                { 0.0f, 0.0f },
                0.0f,
                WHITE
            );
        }

        // ======================================
        // ENEMY BUBBLES
        // ======================================

        for (int j = 0; j < 12; j++)
        {
            if (!enemies[i].bubbles[j].active)
                continue;

            Vector2 bubbleSprite =
            {
                enemies[i].bubbles[j].position.x - playerPos.x,
                enemies[i].bubbles[j].position.y - playerPos.y
            };

            // Camera transformation
            float bubbleInvDet =
                1.0f /
                (cameraPlane.x * playerDir.y -
                    cameraPlane.y * playerDir.x);

            float bubbleTransformX =
                bubbleInvDet *
                (playerDir.y * bubbleSprite.x -
                    playerDir.x * bubbleSprite.y);

            float bubbleTransformY =
                bubbleInvDet *
                (-cameraPlane.y * bubbleSprite.x +
                    cameraPlane.x * bubbleSprite.y);

            // Behind camera
            if (bubbleTransformY <= 0.0f)
                continue;

            int bubbleScreenX =
                (int)(
                    (Config::SCREEN_WIDTH / 2.0f) *
                    (1.0f +
                        bubbleTransformX /
                        bubbleTransformY)
                    );

            int bubbleScreenY =
                Config::SCREEN_HEIGHT / 2;

            float projectedRadius =
                (enemies[i].bubbles[j].radius /
                    bubbleTransformY) *
                2.0f;

            unsigned char alpha =
                (unsigned char)(
                    enemies[i].bubbles[j].life * 255
                    );

            Color bubbleColor =
            {
                173,
                216,
                230,
                alpha
            };

            DrawCircle(
                bubbleScreenX,
                bubbleScreenY,
                projectedRadius,
                bubbleColor
            );
        }
    }
    // ==========================================
// PHASE 2.5: MARKET FOUNTAIN
// ==========================================

    if (insideMarket)
    {
        DrawFountain(
            { 15.5f, 2.5f },
            playerPos,
            playerDir,
            cameraPlane,
            Zbuffer
        );
    }
    // ==========================================
    // PHASE 3: NPCs
    // ==========================================

    for (const NPC& npc : npcs)
    {
        npc.Draw(
            playerPos,
            playerDir,
            cameraPlane,
            Zbuffer,
            Config::SCREEN_WIDTH,
            Config::SCREEN_HEIGHT
        );
    }

    // ==========================================
    // PHASE 4: PLAYER HAND
    // ==========================================

    Texture2D& weaponTex = player.handTex;

    if (weaponTex.id != 0)
    {
        float scale =
            (Config::SCREEN_HEIGHT * 0.55f) /
            weaponTex.height;

        float drawWidth =
            weaponTex.width * scale;

        float drawHeight =
            weaponTex.height * scale;

        float bobX;
        float bobY;

        bool walking =
            IsKeyDown(KEY_W) ||
            IsKeyDown(KEY_A) ||
            IsKeyDown(KEY_S) ||
            IsKeyDown(KEY_D);

        if (walking)
        {
            bobX =
                cosf((float)GetTime() * 10.0f) *
                18.0f;

            bobY =
                fabsf(sinf((float)GetTime() * 10.0f)) *
                16.0f;
        }
        else
        {
            bobX = 0.0f;

            bobY =
                sinf((float)GetTime() * 2.0f) *
                3.0f;
        }

        float drawX =
            Config::SCREEN_WIDTH / 2.0f -
            drawWidth / 2.0f +
            bobX;

        float drawY =
            Config::SCREEN_HEIGHT -
            drawHeight +
            100.0f +
            bobY;

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
            { 0.0f, 0.0f },
            0.0f,
            WHITE
        );
    }

    // ==========================================
    // DOOR MARKERS
    // ==========================================

    DrawDoorMarker(
        playerPos,
        playerDir,
        cameraPlane,
        Zbuffer,
        { 9.5f, 2.5f }
    );

    DrawDoorMarker(
        playerPos,
        playerDir,
        cameraPlane,
        Zbuffer,
        { 24.5f, 23.5f }
    );

    // ==========================================
    // HIT MESSAGE
    // ==========================================

    if (player.hitmessagetimer > 0)
    {
        DrawText(
            "BAM! UNCLE HIT!",
            GetScreenWidth() / 2 - 150,
            GetScreenHeight() / 2 + 100,
            40,
            GREEN
        );
    }
    DrawHud(player);
    DrawInventoryHUD(player);
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
        - markerHeight
        - 20
        + (int)bob;
    if (screenX < 0 || screenX >= Config::SCREEN_WIDTH)
        return;

    Color color =
        (((int)(GetTime() * 4) % 2) == 0)
        ? YELLOW
        : ORANGE;

    DrawTriangle(
        {
            (float)screenX,
            (float)(drawY - markerHeight)
        },
{
    (float)(screenX - markerWidth / 2),
    (float)drawY
},
{
    (float)(screenX + markerWidth / 2),
    (float)drawY
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
void Renderer::DrawFountain(
    Vector2 fountainPos,
    Vector2 playerPos,
    Vector2 playerDir,
    Vector2 cameraPlane,
    float Zbuffer[])
{
    Texture2D& sheet = textures.fountainSheet;

    if (sheet.id == 0)
        return;

    // ==========================================
    // POSITION RELATIVE TO PLAYER
    // ==========================================

    Vector2 sprite =
    {
        fountainPos.x - playerPos.x,
        fountainPos.y - playerPos.y
    };

    // ==========================================
    // CAMERA TRANSFORMATION
    // ==========================================

    float invDet =
        1.0f /
        (cameraPlane.x * playerDir.y -
            cameraPlane.y * playerDir.x);

    float transformX =
        invDet *
        (playerDir.y * sprite.x -
            playerDir.x * sprite.y);

    float transformY =
        invDet *
        (-cameraPlane.y * sprite.x +
            cameraPlane.x * sprite.y);

    // Behind player
    if (transformY <= 0.0f)
        return;

    // ==========================================
    // ANIMATION FRAME
    // ==========================================

    int currentFrame =
        textures.GetFountainFrame();

    float frameWidth =
        (float)sheet.width / 4.0f;

    float frameHeight =
        (float)sheet.height;

    float frameOffsetX =
        currentFrame * frameWidth;

    // ==========================================
    // PROJECT SPRITE
    // ==========================================

    int spriteScreenX =
        (int)(
            (Config::SCREEN_WIDTH / 2.0f) *
            (1.0f + transformX / transformY)
            );

    // Fountain size
    int spriteHeight =
        abs(
            (int)(
                Config::SCREEN_HEIGHT /
                transformY
                )
        );

    // Keep fountain proportional to its frame
    float aspectRatio =
        frameWidth / frameHeight;

    int spriteWidth =
        (int)(spriteHeight * aspectRatio);

    // Make fountain a little bigger
    spriteHeight *= 1.2f;
    spriteWidth *= 1.2f;

    // ==========================================
    // VERTICAL POSITION
    // ==========================================

    int drawStartY =
        -spriteHeight / 2 +
        Config::SCREEN_HEIGHT / 2;

    int drawEndY =
        spriteHeight / 2 +
        Config::SCREEN_HEIGHT / 2;

    if (drawStartY < 0)
        drawStartY = 0;

    if (drawEndY >= Config::SCREEN_HEIGHT)
        drawEndY =
        Config::SCREEN_HEIGHT - 1;

    // ==========================================
    // HORIZONTAL POSITION
    // ==========================================

    int drawStartX =
        -spriteWidth / 2 +
        spriteScreenX;

    int drawEndX =
        spriteWidth / 2 +
        spriteScreenX;

    if (drawStartX < 0)
        drawStartX = 0;

    if (drawEndX >= Config::SCREEN_WIDTH)
        drawEndX =
        Config::SCREEN_WIDTH - 1;

    // Completely outside screen
    if (drawStartX >= Config::SCREEN_WIDTH ||
        drawEndX < 0)
    {
        return;
    }

    // ==========================================
    // DRAW STRIPE BY STRIPE
    // ==========================================

    for (int stripe = drawStartX;
        stripe < drawEndX;
        stripe++)
    {
        if (stripe < 0 ||
            stripe >= Config::SCREEN_WIDTH)
        {
            continue;
        }

        // ======================================
        // WALL Z-BUFFER
        // ======================================

        if (transformY >= Zbuffer[stripe])
            continue;

        // ======================================
        // TEXTURE X
        // ======================================

        int trueStartX =
            -spriteWidth / 2 +
            spriteScreenX;

        int texX =
            (int)(
                (stripe - trueStartX) *
                frameWidth /
                spriteWidth
                );

        if (texX < 0)
            texX = 0;

        if (texX >= (int)frameWidth)
            texX =
            (int)frameWidth - 1;

        // ======================================
        // SOURCE
        // ======================================

        Rectangle sourceRec =
        {
            frameOffsetX + texX,
            0.0f,
            1.0f,
            frameHeight
        };

        // ======================================
        // DESTINATION
        // ======================================

        Rectangle destRec =
        {
            (float)stripe,
            (float)drawStartY,
            1.0f,
            (float)(drawEndY - drawStartY)
        };

        DrawTexturePro(
            sheet,
            sourceRec,
            destRec,
            { 0.0f, 0.0f },
            0.0f,
            WHITE
        );
    }
}
