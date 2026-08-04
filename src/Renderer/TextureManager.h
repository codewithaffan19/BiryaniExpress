#pragma once

#include "raylib.h"

class TextureManager
{
public:

    void Load();

    void Unload();

    Texture2D wall;

    Color* wallPixels;
};