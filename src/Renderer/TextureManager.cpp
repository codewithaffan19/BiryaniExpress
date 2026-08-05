#include "TextureManager.h"
#include "raylib.h"
#include<string>
void TextureManager::Load()
{
    for (int i = 0; i < 10; i++)
    {
        walls[i].id = 0;
        wallPixels[i] = nullptr;
    }

    for (int i = 1; i <= 3; i++)   // <-- only load existing walls
    {
        std::string path =
            "../assets/textures/wall" +
            std::to_string(i) +
            ".png";

        walls[i + 1] = LoadTexture(path.c_str());

        Image img = LoadImageFromTexture(walls[i + 1]);
        wallPixels[i + 1] = LoadImageColors(img);
        UnloadImage(img);
    }
}
void TextureManager::Unload()
{
    for (int i = 1; i <= 9; i++)
    {
        if (wallPixels[i])
            UnloadImageColors(wallPixels[i]);

        if (walls[i].id != 0)
            UnloadTexture(walls[i]);
    }
}