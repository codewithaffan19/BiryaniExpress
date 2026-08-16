#include "TextureManager.h"
#include "raylib.h"

#include <string>
// =============================================================
// CONSTRUCTOR
// =============================================================

TextureManager::TextureManager()
{
    // ---------------------------------------------------------
    // Default everything to Street.
    // ---------------------------------------------------------

    for (int i = 0; i < MAX_TILES; i++)
    {
        tilePartitions[i] = Partition::Street;
    }

    // ---------------------------------------------------------
    // MARKET
    //
    // 10 - 19
    // 22 - 30
    // ---------------------------------------------------------

    for (int i = 10; i <= 19; i++)
    {
        tilePartitions[i] = Partition::Market;
    }

    for (int i = 22; i <= 30; i++)
    {
        tilePartitions[i] = Partition::Market;
    }

    // ---------------------------------------------------------
    // BASIC TILE NAMES
    // ---------------------------------------------------------

    tileNames[0] = "Empty";

    // ---------------------------------------------------------
    // COMMON STREET -> MARKET DOORS
    //
    // 20 = EntranceDoor
    // 21 = ExitDoor
    //
    // KEEP THESE.
    // ---------------------------------------------------------

    tileNames[20] = "EntranceDoor";
    tileNames[21] = "ExitDoor";

    // ---------------------------------------------------------
    // COMMON KEY / PARTITION DOORS
    //
    // 42 = Door1
    // 43 = Door2
    // 44 = Door3
    // 45 = NeonDoor
    //
    // These are normal single-frame tiles.
    // No animation.
    // ---------------------------------------------------------

    tileNames[42] = "Door1";
    tileNames[43] = "Door2";
    tileNames[44] = "Door3";
    tileNames[45] = "NeonDoor";
    tileNames[63] = "StreetTile1";
    tileNames[64] = "StreetTile2";
    // =========================================================
// NEON NIGHT WALLS
//
// 47 - 61 = NeonWall1 - NeonWall15
// No animation.
// =========================================================

    for (int i = 47; i <= 62; i++)
    {
        tileNames[i] =
            "NeonWall" + std::to_string(i - 46);

        tilePartitions[i] =
            Partition::NeonNight;
    }
    LoadCommonAssets();
}
// =============================================================
// HELPERS
// =============================================================
void TextureManager::SetCurrentPartition(Partition partition)
{
    if (currentPartition == partition)
        return;

    // Unload the currently active partition.
    switch (currentPartition)
    {
    case Partition::Street:
        UnloadStreetAssets();
        break;

    case Partition::Market:
        UnloadMarketAssets();
        break;

    case Partition::NeonNight:
        UnloadNeonNightAssets();
        break;
    }

    // Load the requested partition.
    switch (partition)
    {
    case Partition::Street:
        LoadStreetAssets();
        break;

    case Partition::Market:
        LoadMarketAssets();
        break;

    case Partition::NeonNight:
        LoadNeonNightAssets();
        break;
    }

    currentPartition = partition;
}
void TextureManager::LoadTile(
    int index,
    const std::string& path,
    const std::string& name,
    bool loadPixels)
{
    if (index < 0 || index >= MAX_TILES)
        return;

    if (tiles[index].id != 0)
        return;

    tiles[index] = LoadTexture(path.c_str());

    if (tiles[index].id == 0)
    {
        TraceLog(
            LOG_ERROR,
            "Failed to load texture: %s",
            path.c_str()
        );

        return;
    }

    SetTextureFilter(
        tiles[index],
        TEXTURE_FILTER_POINT
    );

    tileNames[index] = name;

    // Only load CPU pixels when required.
    if (loadPixels)
    {
        Image img = LoadImageFromTexture(tiles[index]);

        if (img.data != nullptr)
        {
            tilePixels[index] = LoadImageColors(img);
            UnloadImage(img);
        }
    }
}

// =============================================================
// PARTITION CHECK
// =============================================================

bool TextureManager::IsTileInPartition(
    int tile,
    Partition partition
) const
{
    if (tile < 0 || tile >= MAX_TILES)
        return false;

    // Common assets are available in every partition.
    if (tile == 20 ||
        tile == 21 ||
        tile == 42 ||
        tile == 43 ||
        tile == 44 ||
        tile == 45)
    {
        return true;
    }

    return tilePartitions[tile] == partition;
}

// =============================================================
// ANIMATED TILE
// =============================================================

void TextureManager::LoadAnimatedTile(
    int index,
    const std::string& path)
{
    if (index < 0 || index >= MAX_TILES)
        return;

    if (!FileExists(path.c_str()))
        return;

    if (tileAnim[index].id != 0)
        return;

    tileAnim[index] = LoadTexture(path.c_str());

    if (tileAnim[index].id != 0)
    {
        SetTextureFilter(
            tileAnim[index],
            TEXTURE_FILTER_POINT
        );
    }
}

// =============================================================
// UNLOAD TILE
// =============================================================

void TextureManager::UnloadTile(int index)
{
    if (index < 0 || index >= MAX_TILES)
        return;

    if (tilePixels[index] != nullptr)
    {
        UnloadImageColors(tilePixels[index]);
        tilePixels[index] = nullptr;
    }

    if (tiles[index].id != 0)
    {
        UnloadTexture(tiles[index]);
        tiles[index].id = 0;
    }

    if (tileAnim[index].id != 0)
    {
        UnloadTexture(tileAnim[index]);
        tileAnim[index].id = 0;
    }

    tileNames[index].clear();
}

// =============================================================
// CHECK
// =============================================================

bool TextureManager::IsTileLoaded(int index) const
{
    if (index < 0 || index >= MAX_TILES)
        return false;

    return tiles[index].id != 0;
}

// =============================================================
// STREET ASSETS
// =============================================================

void TextureManager::LoadStreetAssets()
{
    TraceLog(LOG_INFO, "Loading STREET assets...");
    // =========================================================
// STREET TILE PARTITIONS
// =========================================================

    for (int i = 0; i < MAX_TILES; i++)
    {
        tilePartitions[i] = Partition::Street;
    }

    for (int i = 47; i <= 62; i++)
    {
        tilePartitions[i] = Partition::NeonNight;
    }
    // ---------------------------------------------------------
    // Basic tiles
    // ---------------------------------------------------------

    tileNames[0] = "Empty";

    // NOTE: tile 1 ("RedWall", the old hardcoded placeholder wall)
    // and tiles 2-9 ("Wall1"-"Wall8", wall1.png-wall8.png) are
    // removed â€” unused, not placed anywhere in the finished map.

    // ---------------------------------------------------------
    // Welcome Wall
    // ---------------------------------------------------------

    LoadTile(
        31,
        "assets/streetassets/wall_welcome.png",
        "WallWelcome",
        true
    );

    // ---------------------------------------------------------
    // Street Walls
    //
    // Only 1-6 exist on disk (streetwall7-10.png are missing,
    // so those slots are skipped instead of being retried
    // every load).
    // ---------------------------------------------------------

    for (int i = 0; i < 6; i++)
    {
        int index = 32 + i;

        std::string path =
            "assets/streetassets/streetwall" +
            std::to_string(i + 1) +
            ".png";

        LoadTile(
            index,
            path,
            "StreetWall" + std::to_string(i + 1),
            true
        );

        std::string animPath =
            "assets/streetassets/streetwall" +
            std::to_string(i + 1) +
            "_anim.png";

        LoadAnimatedTile(
            index,
            animPath
        );
    }

    // ---------------------------------------------------------
    // Street Floor
    // ---------------------------------------------------------

    if (floorimg.data == nullptr)
    {
        floorimg =
            LoadImage(
                "assets/streetassets/floor.png"
            );

        // Force a known pixel layout so the fast per-pixel fetch
        // used by the floor caster (GetPixelFast in Renderer.cpp)
        // can read raw bytes safely, regardless of the source
        // PNG's original color format.
        ImageFormat(
            &floorimg,
            PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
        );
    }

    if (floor2Img.data == nullptr)
    {
        floor2Img =
            LoadImage(
                "assets/streetassets/floor2.png"
            );

        ImageFormat(
            &floor2Img,
            PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
        );
    }

    // ---------------------------------------------------------
    // Street Sky
    // ---------------------------------------------------------

    if (skyTex.id == 0)
    {
        skyTex =
            LoadTexture(
                "assets/streetassets/sky.png"
            );

        SetTextureFilter(
            skyTex,
            TEXTURE_FILTER_BILINEAR
        );
    }
    // ---------------------------------------------------------
// Extra street tiles
// ---------------------------------------------------------

    LoadTile(
        63,
        "assets/streetassets/yourtile1.png",
        "StreetTile1",
        true
    );

    LoadTile(
        64,
        "assets/streetassets/yourtile2.png",
        "StreetTile2",
        true
    );
    currentPartition = Partition::Street;

    TraceLog(LOG_INFO, "STREET assets loaded.");
}

// =============================================================
// MARKET ASSETS
// =============================================================

void TextureManager::LoadMarketAssets()
{
    TraceLog(LOG_INFO, "Loading MARKET assets...");
    // =========================================================
// MARKET TILE PARTITIONS
// =========================================================

    const int marketTiles[] =
    {
        10, 11, 12, 13, 14,
        15, 16, 17, 18, 19,
        22, 23, 24, 25, 26,
        27, 28, 29, 30
    };

    for (int tile : marketTiles)
    {
        tilePartitions[tile] = Partition::Market;
    }
    // ---------------------------------------------------------
    // Market Shops
    // ---------------------------------------------------------

    LoadTile(
        10,
        "assets/marketassets/6.3cafe.png",
        "Cafe"
    );

    LoadTile(
        11,
        "assets/marketassets/AFC.png",
        "AFC"
    );

    LoadTile(
        12,
        "assets/marketassets/drumble.png",
        "Drumble"
    );

    // NOTE: CHIPS.png and wall8.png (base textures for tiles 13/14)
    // are missing on disk, so those LoadTile() calls were removed â€”
    // the CHIPS_anim / wall8_anim overlays below still load fine
    // and are unaffected.

    LoadTile(
        15,
        "assets/marketassets/marketwall1.png",
        "MarketWall1"
    );

    LoadTile(
        16,
        "assets/marketassets/marketwall2.png",
        "MarketWall2"
    );

    LoadTile(
        17,
        "assets/marketassets/marketwall3.png",
        "MarketWall3"
    );

    LoadTile(
        18,
        "assets/marketassets/AMW.png",
        "AMW"
    );

    LoadTile(
        19,
        "assets/marketassets/SlimeLite.png",
        "SlimeLite"
    );

    // ---------------------------------------------------------
    // Animated Shops
    // ---------------------------------------------------------

    LoadAnimatedTile(
        10,
        "assets/marketassets/6.3cafe_anim.png"
    );

    LoadAnimatedTile(
        11,
        "assets/marketassets/AFC_anim.png"
    );

    LoadAnimatedTile(
        12,
        "assets/marketassets/drumble_anim.png"
    );

    LoadAnimatedTile(
        13,
        "assets/marketassets/CHIPS_anim.png"
    );

    LoadAnimatedTile(
        14,
        "assets/marketassets/wall8_anim.png"
    );

    LoadAnimatedTile(
        18,
        "assets/marketassets/AMW_anim.png"
    );

    LoadAnimatedTile(
        19,
        "assets/marketassets/SlimeLite_anim.png"
    );
    LoadTile(
        20,
        "assets/commonassets/entranceDoor.png",
        "EntranceDoor"
    );

    LoadTile(
        21,
        "assets/commonassets/exitDoor.png",
        "ExitDoor"
    );
    // ---------------------------------------------------------
    // Extra Shops
    // ---------------------------------------------------------

    LoadTile(
        22,
        "assets/marketassets/gym.png",
        "Gym"
    );

    LoadAnimatedTile(
        22,
        "assets/marketassets/gym_anim.png"
    );

    LoadTile(
        23,
        "assets/marketassets/mike.png",
        "Mike"
    );

    LoadAnimatedTile(
        23,
        "assets/marketassets/mike_anim.png"
    );

    LoadTile(
        24,
        "assets/marketassets/kiko_milano.png",
        "KikoMilano"
    );

    LoadAnimatedTile(
        24,
        ".assets/marketassets/kiko_milano_anim.png"
    );

    LoadTile(
        25,
        "assets/marketassets/mehak_posh.png",
        "MehakPosh"
    );

    LoadAnimatedTile(
        25,
        "assets/marketassets/mehak_posh_anim.png"
    );

    LoadTile(
        26,
        "assets/marketassets/clinex.png",
        "Clinex"
    );

    LoadAnimatedTile(
        26,
        "assets/marketassets/clinex_anim.png"
    );

    // ---------------------------------------------------------
    // Frame 2 Shops
    // ---------------------------------------------------------

    LoadTile(
        27,
        "assets/marketassets/bar.png",
        "Bar"
    );

    LoadAnimatedTile(
        27,
        "assets/marketassets/bar_anim.png"
    );

    LoadTile(
        28,
        "assets/marketassets/brolex.png",
        "Brolex"
    );

    LoadAnimatedTile(
        28,
        "assets/marketassets/brolex_anim.png"
    );

    LoadTile(
        29,
        "assets/marketassets/Davidputra.png",
        "DavidPutra"
    );

    LoadAnimatedTile(
        29,
        "assets/marketassets/Davidputra_anim.png"
    );

    LoadTile(
        30,
        "assets/marketassets/gamingzone.png",
        "GamingZone"
    );

    LoadAnimatedTile(
        30,
        "assets/marketassets/gamingzone_anim.png"
    );

    // ---------------------------------------------------------
    // Market Floor
    // ---------------------------------------------------------

    if (marketFloorImg.data == nullptr)
    {
        marketFloorImg =
            LoadImage(
                "assets/marketassets/marketFloor.png"
            );

        ImageFormat(
            &marketFloorImg,
            PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
        );
    }

    // ---------------------------------------------------------
    // Market Roof
    // ---------------------------------------------------------

    if (marketRoofTex.id == 0)
    {
        marketRoofTex =
            LoadTexture(
                "assets/marketassets/marketRoof.png"
            );

        SetTextureFilter(
            marketRoofTex,
            TEXTURE_FILTER_BILINEAR
        );
    }

    // ---------------------------------------------------------
    // Fountain
    // ---------------------------------------------------------

    if (fountainSheet.id == 0)
    {
        fountainSheet =
            LoadTexture(
                "assets/marketassets/fountain.png"
            );

        SetTextureFilter(
            fountainSheet,
            TEXTURE_FILTER_POINT
        );
    }

    currentPartition = Partition::Market;

    TraceLog(LOG_INFO, "MARKET assets loaded.");
}

void TextureManager::LoadCommonAssets()
{
    // =========================================================
    // COMMON STREET -> MARKET DOORS
    // =========================================================

    LoadTile(
        20,
        "assets/commonassets/entranceDoor.png",
        "EntranceDoor"
    );

    LoadTile(
        21,
        "assets/commonassets/exitDoor.png",
        "ExitDoor"
    );

    // =========================================================
    // KEY LOGIC DOORS
    // =========================================================

    LoadTile(
        42,
        "assets/commonassets/door1.png",
        "Door1"
    );

    LoadTile(
        43,
        "assets/commonassets/door2.png",
        "Door2"
    );

    LoadTile(
        44,
        "assets/commonassets/door3.png",
        "Door3"
    );

    // =========================================================
    // STREET <-> NEON NIGHT DOOR
    // =========================================================

    LoadTile(
        45,
        "assets/commonassets/neonDoor.png",
        "NeonDoor"
    );
    LoadTile(
        46,
        "assets/commonassets/marketdoor.png",
        "NeonDoor"
    );

}
// =============================================================
// NEON NIGHT ASSETS
// =============================================================

void TextureManager::LoadNeonNightAssets()
{
    TraceLog(LOG_INFO, "Loading NEON NIGHT assets...");

    // ---------------------------------------------------------
    // Neon Night floor
    // ---------------------------------------------------------

    if (nightFloorImg.data == nullptr)
    {
        nightFloorImg =
            LoadImage(
                "assets/neonnightassets/nightFloor.png"
            );

        ImageFormat(
            &nightFloorImg,
            PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
        );
    }

    // ---------------------------------------------------------
    // Neon Night sky
    // ---------------------------------------------------------

    if (nightSkyTex.id == 0)
    {
        nightSkyTex =
            LoadTexture(
                "assets/neonnightassets/nightSky.png"
            );

        SetTextureFilter(
            nightSkyTex,
            TEXTURE_FILTER_BILINEAR
        );
    }

    currentPartition = Partition::NeonNight;

    TraceLog(LOG_INFO, "NEON NIGHT assets loaded.");
    // =========================================================
// NEON NIGHT WALLS
//
// Tile IDs 47 - 61
// No animation.
// =========================================================

    for (int i = 1; i <= 16; i++)
    {
        int index = 46 + i;

        std::string path =
            "assets/neonnightassets/neonwall" +
            std::to_string(i) +
            ".png";

        LoadTile(
            index,
            path,
            "NeonWall" + std::to_string(i),
            true
        );
    }
}
// =============================================================
// UNLOAD STREET
// =============================================================

void TextureManager::UnloadStreetAssets()
{
    TraceLog(LOG_INFO, "Unloading STREET assets...");

    // ---------------------------------------------------------
    // Street tile ranges
    //
    // 31      = Welcome wall
    // 32 - 41 = Street walls
    //
    // IMPORTANT:
    // 20 and 21 are doors and are COMMON.
    // They are deliberately NOT unloaded here.
    // ---------------------------------------------------------

    UnloadTile(31);

    for (int i = 32; i <= 41; i++)
        UnloadTile(i);
    UnloadTile(63);
    UnloadTile(64);
    // ---------------------------------------------------------
    // Street Floor
    // ---------------------------------------------------------

    if (floorimg.data != nullptr)
    {
        UnloadImage(floorimg);
        floorimg = {};
    }

    if (floor2Img.data != nullptr)
    {
        UnloadImage(floor2Img);
        floor2Img = {};
    }

    // ---------------------------------------------------------
    // Street Sky
    // ---------------------------------------------------------

    if (skyTex.id != 0)
    {
        UnloadTexture(skyTex);
        skyTex.id = 0;
    }
}

// =============================================================
// UNLOAD MARKET
// =============================================================

void TextureManager::UnloadMarketAssets()
{
    TraceLog(LOG_INFO, "Unloading MARKET assets...");

    // ---------------------------------------------------------
    // Market tiles
    //
    // 10 - 19
    // 22 - 30
    //
    // 20 and 21 are COMMON doors.
    // They are deliberately NOT unloaded.
    // ---------------------------------------------------------

    for (int i = 10; i <= 19; i++)
        UnloadTile(i);

    for (int i = 22; i <= 30; i++)
        UnloadTile(i);

    // ---------------------------------------------------------
    // Market Floor
    // ---------------------------------------------------------

    if (marketFloorImg.data != nullptr)
    {
        UnloadImage(marketFloorImg);
        marketFloorImg = {};
    }

    // ---------------------------------------------------------
    // Market Roof
    // ---------------------------------------------------------

    if (marketRoofTex.id != 0)
    {
        UnloadTexture(marketRoofTex);
        marketRoofTex.id = 0;
    }

    // ---------------------------------------------------------
    // Fountain
    // ---------------------------------------------------------

    if (fountainSheet.id != 0)
    {
        UnloadTexture(fountainSheet);
        fountainSheet.id = 0;
    }
}
void TextureManager::UnloadNeonNightAssets()
{
    TraceLog(LOG_INFO, "Unloading NEON NIGHT assets...");
    // =========================================================
// NEON NIGHT WALLS
// =========================================================

    for (int i = 47; i <= 62; i++)
    {
        UnloadTile(i);
    }
    if (nightFloorImg.data != nullptr)
    {
        UnloadImage(nightFloorImg);
        nightFloorImg = {};
    }

    if (nightSkyTex.id != 0)
    {
        UnloadTexture(nightSkyTex);
        nightSkyTex.id = 0;
    }
}

// =============================================================
// TEMPORARY LEGACY LOAD
// =============================================================

void TextureManager::Load()
{
    // Temporary compatibility function.
    //
    // The actual partition system will eventually
    // replace this completely.

    LoadStreetAssets();

    // IMPORTANT:
    // We do NOT load Market here anymore.
    //
    // This means starting the game in Street does not
    // immediately load Market assets.

    currentPartition = Partition::Street;
}

// =============================================================
// TEMPORARY LEGACY UNLOAD
// =============================================================

void TextureManager::Unload()
{
    UnloadStreetAssets();
    UnloadMarketAssets();
    UnloadNeonNightAssets();

    // ---------------------------------------------------------
    // Common doors
    // ---------------------------------------------------------

    UnloadTile(20);
    UnloadTile(21);

    UnloadTile(42);
    UnloadTile(43);
    UnloadTile(44);
    UnloadTile(45);
    UnloadTile(46);
    currentPartition = Partition::Street;
}
// =============================================================
// FOUNTAIN ANIMATION
// =============================================================

int TextureManager::GetFountainFrame()
{
    fountainFrameTimer += GetFrameTime();

    const float frameSpeed = 6.0f;

    if (fountainFrameTimer >= 1.0f / frameSpeed)
    {
        fountainFrameTimer = 0.0f;

        fountainCurrentFrame++;

        if (fountainCurrentFrame >= 4)
            fountainCurrentFrame = 0;
    }

    return fountainCurrentFrame;
}