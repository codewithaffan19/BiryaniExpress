#pragma once

#include "raylib.h"

class TextureManager
{
public:

    void Load();

    void Unload();

    Texture2D wall;
    Texture2D wallTexture1;

    Color* wallPixels;
    Color* wallTexture1Pixels;
};