#pragma once

#include "raylib.h"
#include <string>

class TextureManager
{
public:
    TextureManager();
    // =========================================================
    // COMMON ASSETS
    // =========================================================

    void LoadCommonAssets();
    // =========================================================
    // PARTITIONS
    // =========================================================

    enum class Partition
    {
        Street,
        Market,
        NeonNight
    };

    // =========================================================
    // TILE DATA
    // =========================================================

    static constexpr int MAX_TILES = 128;
    Texture2D neonDoorTex{};
    Texture2D neonDoorOpenTex{};
    Texture2D neonDoor1Tex{};
    Texture2D neonDoor2Tex{};
    Texture2D tiles[MAX_TILES]{};
    Texture2D tileAnim[MAX_TILES]{};
    Color* tilePixels[MAX_TILES]{};

    std::string tileNames[MAX_TILES];
    Partition tilePartitions[MAX_TILES]{};
    // =========================================================
    // CURRENT / LEGACY LOADING
    // =========================================================

    // Temporary compatibility function.
    // We will stop using this once Renderer/Game loading
    // is converted to partition loading.
    void Load();

    void Unload();

    // =========================================================
    // PARTITION LOADING
    // =========================================================

    void LoadStreetAssets();
    void LoadMarketAssets();
    void LoadNeonNightAssets();
    bool IsTileInPartition(int tileIndex, Partition partition) const;
    void UnloadStreetAssets();
    void UnloadMarketAssets();
    void UnloadNeonNightAssets();

    // =========================================================
    // FLOOR / SKY
    // =========================================================

    Image floorimg{};
    Image floor2Img{};

    Image marketFloorImg{};

    Texture2D skyTex{};
    Texture2D marketRoofTex{};

    // These will be used for the Neon Night section.
    Image nightFloorImg{};
    Texture2D nightSkyTex{};

    // =========================================================
    // MARKET OBJECTS
    // =========================================================

    Texture2D fountainSheet{};

    int fountainCurrentFrame = 0;
    float fountainFrameTimer = 0.0f;

    int GetFountainFrame();

    // =========================================================
    // CURRENT PARTITION
    // =========================================================

    void SetCurrentPartition(Partition partition);

    Partition GetCurrentPartition() const
    {
        return currentPartition;
    }

private:

    Partition currentPartition = Partition::Street;

    // =========================================================
    // HELPERS
    // =========================================================

    void LoadTile(
        int index,
        const std::string& path,
        const std::string& name,
        bool loadPixels = false
    );

    void LoadAnimatedTile(
        int index,
        const std::string& path
    );

    void UnloadTile(int index);

    bool IsTileLoaded(int index) const;
};