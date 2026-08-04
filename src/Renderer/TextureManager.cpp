#include "TextureManager.h"
#include "raylib.h"
void TextureManager::Load()
{
    wall = LoadTexture("assets/textures/wall.png");
    wallTexture1 = LoadTexture("assets/textures/wall1.png");

    Image img = LoadImageFromTexture(wall);
    wallPixels = LoadImageColors(img);
    UnloadImage(img);

    Image img2 = LoadImageFromTexture(wallTexture1);
    wallTexture1Pixels = LoadImageColors(img2);
    UnloadImage(img2);
}

void TextureManager::Unload()
{
    UnloadImageColors(wallPixels);
    UnloadTexture(wall);

    UnloadImageColors(wallTexture1Pixels);
    UnloadTexture(wallTexture1);
}