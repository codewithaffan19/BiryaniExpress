#pragma once

#include "raylib.h"
#include <string>

class TextureManager
{
public:

    void Load();
    void Unload();

    static constexpr int MAX_TILES = 128;

    Texture2D tiles[MAX_TILES]{};
    Texture2D tileAnim[MAX_TILES]{};
    Color* tilePixels[MAX_TILES]{};

    Texture2D skyTex;
    Texture2D marketRoofTex;

    Image floorimg;
    Image marketFloorImg;

    std::string tileNames[MAX_TILES];

    Texture2D fountainSheet{};
    int fountainCurrentFrame = 0;
    float fountainFrameTimer = 0.0f;
    int GetFountainFrame();
};