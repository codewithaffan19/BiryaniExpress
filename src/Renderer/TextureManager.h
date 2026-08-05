#pragma once

#include "raylib.h"

class TextureManager
{
public:

    void Load();

    void Unload();

    Texture2D walls[10];
    Color* wallPixels[10];
};