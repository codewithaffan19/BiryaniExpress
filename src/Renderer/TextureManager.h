#pragma once
#include<string>
#include "raylib.h"
class TextureManager
{
public:

    void Load();

    void Unload();

    Image floorimg;
    static constexpr int MAX_TILES = 64;

    Texture2D tiles[MAX_TILES]{};
    Texture2D tileAnim[MAX_TILES]{};
    Color* tilePixels[MAX_TILES]{};
    Texture2D skyTex;
    Texture2D door[3];
    std::string tileNames[MAX_TILES];
};