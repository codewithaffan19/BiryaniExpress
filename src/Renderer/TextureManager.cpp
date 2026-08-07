#include "TextureManager.h"
#include "raylib.h"
#include<string>
void TextureManager::Load()
{
    //==========================
    // Basic tiles
    //==========================

    tileNames[0] = "Empty";
    tileNames[1] = "RedWall";

    //==========================
    // Walls
    //==========================

    for (int i = 1; i <= 8; i++)
    {
        int index = i + 1;

        // ---------- Normal frame ----------
        std::string path =
            "../../assets/textures/wall" +
            std::to_string(i) +
            ".png";

        tiles[index] = LoadTexture(path.c_str());
        SetTextureFilter(tiles[index], TEXTURE_FILTER_POINT);

        Image img = LoadImageFromTexture(tiles[index]);
        tilePixels[index] = LoadImageColors(img);
        UnloadImage(img);

        // ---------- Animation frame ----------
        std::string animPath =
            "../../assets/textures/wall" +
            std::to_string(i) +
            "_anim.png";

        if (FileExists(animPath.c_str()))
        {
            tileAnim[index] = LoadTexture(animPath.c_str());
            SetTextureFilter(tileAnim[index], TEXTURE_FILTER_POINT);
        }

        tileNames[index] = "Wall" + std::to_string(i);
    }

    //==========================
    // Shops
    //==========================

    tiles[10] = LoadTexture("../../assets/textures/6.3cafe.png");
    tileNames[10] = "Cafe";

    tiles[11] = LoadTexture("../../assets/textures/AFC.png");
    tileNames[11] = "AFC";

    tiles[12] = LoadTexture("../../assets/textures/drumble.png");
    tileNames[12] = "Drumble";

    tiles[13] = LoadTexture("../../assets/textures/CHIPS.png");
    tileNames[13] = "CHIPS";

    tiles[14] = LoadTexture("../../assets/textures/wall8.png");
    tileNames[14] = "Butcher";

    tiles[15] = LoadTexture("../../assets/textures/marketwall1.png");
    tileNames[15] = "MarketWall1";

    tiles[16] = LoadTexture("../../assets/textures/marketwall2.png");
    tileNames[16] = "MarketWall2";

    tiles[17] = LoadTexture("../../assets/textures/marketwall3.png");
    tileNames[17] = "MarketWall3";

    tiles[18] = LoadTexture("../../assets/textures/AMW.png");
    tileNames[18] = "AMW";

    tiles[19] = LoadTexture("../../assets/textures/SlimeLite.png");
    tileNames[19] = "SlimeLite";

    //==========================
    // Animation Frames
    //==========================

    tileAnim[10] = LoadTexture("../../assets/textures/6.3cafe_anim.png");
    tileAnim[11] = LoadTexture("../../assets/textures/AFC_anim.png");
    tileAnim[12] = LoadTexture("../../assets/textures/drumble_anim.png");
    tileAnim[13] = LoadTexture("../../assets/textures/CHIPS_anim.png");
    tileAnim[14] = LoadTexture("../../assets/textures/wall8_anim.png");
    tileAnim[18] = LoadTexture("../../assets/textures/AMW_anim.png");
    tileAnim[19] = LoadTexture("../../assets/textures/SlimeLite_anim.png");

    //==========================
    // Floor
    //==========================

    floorimg = LoadImage("../../assets/textures/floor.png");

    //==========================
    // Sky
    //==========================

    skyTex = LoadTexture("../../assets/textures/sky.png");
    SetTextureFilter(skyTex, TEXTURE_FILTER_BILINEAR);
    //==========================
    // Door
    //==========================
    door[0] = LoadTexture("../../assets/textures/door.png");
    door[1] = LoadTexture("../../assets/textures/door2.png");
    door[2] = LoadTexture("../../assets/textures/door3.png");

    for (int i = 0; i < 3; i++)
        SetTextureFilter(door[i], TEXTURE_FILTER_BILINEAR);
}
void TextureManager::Unload()
{
    for (int i = 0; i < MAX_TILES; i++)
    {
        if (tilePixels[i] != nullptr)
        {
            UnloadImageColors(tilePixels[i]);
            tilePixels[i] = nullptr;
        }

        if (tiles[i].id != 0)
        {
            UnloadTexture(tiles[i]);
            tiles[i].id = 0;
        }

        if (tileAnim[i].id != 0)
        {
            UnloadTexture(tileAnim[i]);
            tileAnim[i].id = 0;
        }
    }

    if (floorimg.data != nullptr)
        UnloadImage(floorimg);

    if (skyTex.id != 0)
        UnloadTexture(skyTex);
    for (int i = 0; i < 3; i++)
    {
        if (door[i].id != 0)
            UnloadTexture(door[i]);
    }
}