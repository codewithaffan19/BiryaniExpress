#include "TextureManager.h"

void TextureManager::Load()
{
    wall = LoadTexture("assets/textures/wall.png");

    Image img = LoadImageFromTexture(wall);

    wallPixels = LoadImageColors(img);

    UnloadImage(img);
}

void TextureManager::Unload()
{
    UnloadImageColors(wallPixels);

    UnloadTexture(wall);
}