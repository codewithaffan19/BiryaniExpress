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
            "../assets/textures/wall" +
            std::to_string(i) +
            ".png";

        tiles[index] = LoadTexture(path.c_str());
        SetTextureFilter(tiles[index], TEXTURE_FILTER_POINT);

        Image img = LoadImageFromTexture(tiles[index]);
        tilePixels[index] = LoadImageColors(img);
        UnloadImage(img);

        // ---------- Animation frame ----------
        std::string animPath =
            "../assets/textures/wall" +
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

    tiles[10] = LoadTexture("../assets/textures/6.3cafe.png");
    tileNames[10] = "Cafe";

    tiles[11] = LoadTexture("../assets/textures/AFC.png");
    tileNames[11] = "AFC";

    tiles[12] = LoadTexture("../assets/textures/drumble.png");
    tileNames[12] = "Drumble";

    tiles[13] = LoadTexture("../assets/textures/CHIPS.png");
    tileNames[13] = "CHIPS";

    tiles[14] = LoadTexture("../assets/textures/wall8.png");
    tileNames[14] = "Butcher";

    tiles[15] = LoadTexture("../assets/textures/marketwall1.png");
    tileNames[15] = "MarketWall1";

    tiles[16] = LoadTexture("../assets/textures/marketwall2.png");
    tileNames[16] = "MarketWall2";

    tiles[17] = LoadTexture("../assets/textures/marketwall3.png");
    tileNames[17] = "MarketWall3";

    tiles[18] = LoadTexture("../assets/textures/AMW.png");
    tileNames[18] = "AMW";

    tiles[19] = LoadTexture("../assets/textures/SlimeLite.png");
    tileNames[19] = "SlimeLite";

    //==========================
    // Door
    //==========================

    tiles[20] = LoadTexture("../assets/textures/door_idle.png");
    tileNames[20] = "EntranceDoor";

    tiles[21] = LoadTexture("../assets/textures/door_idle.png");
    tileNames[21] = "ExitDoor";

    //==========================
    // Animation Frames
    //==========================

    tileAnim[10] = LoadTexture("../assets/textures/6.3cafe_anim.png");
    tileAnim[11] = LoadTexture("../assets/textures/AFC_anim.png");
    tileAnim[12] = LoadTexture("../assets/textures/drumble_anim.png");
    tileAnim[13] = LoadTexture("../assets/textures/CHIPS_anim.png");
    tileAnim[14] = LoadTexture("../assets/textures/wall8_anim.png");
    tileAnim[18] = LoadTexture("../assets/textures/AMW_anim.png");
    tileAnim[19] = LoadTexture("../assets/textures/SlimeLite_anim.png");
    tileAnim[20] = LoadTexture("../assets/textures/door_stop.png");
    tileAnim[21] = LoadTexture("../assets/textures/door_stop.png");

    //==========================
    // Floor
    //==========================

    floorimg = LoadImage("../assets/textures/floor.png");

    //==========================
    // Sky
    //==========================

    skyTex = LoadTexture("../assets/textures/sky.png");
    marketFloorImg =
        LoadImage("../assets/textures/marketFloor.png");

    marketRoofTex =
        LoadTexture("../assets/textures/marketRoof.png");

    //==========================
// Extra Shops
//==========================

    tiles[22] = LoadTexture("../assets/textures/gym.png");
    tileNames[22] = "Gym";

    tiles[23] = LoadTexture("../assets/textures/mike.png");
    tileNames[23] = "Mike";

    tileAnim[22] = LoadTexture("../assets/textures/gym_anim.png");
    tileAnim[23] = LoadTexture("../assets/textures/mike_anim.png");

    //==========================
// New Shops
//==========================

    tiles[24] = LoadTexture("../assets/textures/kiko_milano.png");
    tileNames[24] = "KikoMilano";

    tileAnim[24] = LoadTexture("../assets/textures/kiko_milano_anim.png");

    tiles[25] = LoadTexture("../assets/textures/mehak_posh.png");
    tileNames[25] = "MehakPosh";

    tileAnim[25] = LoadTexture("../assets/textures/mehak_posh_anim.png");

    tiles[26] = LoadTexture("../assets/textures/clinex.png");
    tileNames[26] = "Clinex";

    tileAnim[26] = LoadTexture("../assets/textures/clinex_anim.png");
    //==========================
// Frame 2 Shops
//==========================

    tiles[27] = LoadTexture("../assets/textures/bar.png");
    tileNames[27] = "Bar";

    tileAnim[27] = LoadTexture("../assets/textures/bar_anim.png");


    tiles[28] = LoadTexture("../assets/textures/brolex.png");
    tileNames[28] = "Brolex";

    tileAnim[28] = LoadTexture("../assets/textures/brolex_anim.png");


    tiles[29] = LoadTexture("../assets/textures/Davidputra.png");
    tileNames[29] = "DavidPutra";

    tileAnim[29] = LoadTexture("../assets/textures/Davidputra_anim.png");


    tiles[30] = LoadTexture("../assets/textures/gamingzone.png");
    tileNames[30] = "GamingZone";

    tileAnim[30] = LoadTexture("../assets/textures/gamingzone_anim.png");
    SetTextureFilter(skyTex, TEXTURE_FILTER_BILINEAR);
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
    if (marketFloorImg.data != nullptr)
        UnloadImage(marketFloorImg);
    if (skyTex.id != 0)
        UnloadTexture(skyTex);
    if (marketRoofTex.id != 0)
        UnloadTexture(marketRoofTex);

}