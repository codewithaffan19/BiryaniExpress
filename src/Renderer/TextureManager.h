#pragma once

#include "raylib.h"

class TextureManager
{
public:

    void Load();

    void Unload();

    Texture2D walls[10];
    Texture2D wallAnim[8];
    Color* wallPixels[10];
};