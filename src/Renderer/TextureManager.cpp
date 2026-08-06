#include "TextureManager.h"
#include "raylib.h"
#include<string>
void TextureManager::Load()
{
    for (int i = 1; i <= 8; i++)
    {
        std::string path =
            "../assets/textures/wall" +
            std::to_string(i) +
            ".png";

        walls[i - 1] = LoadTexture(path.c_str());
        SetTextureFilter(walls[i - 1], TEXTURE_FILTER_POINT);
        Image img = LoadImageFromTexture(walls[i - 1]);
        wallPixels[i - 1] = LoadImageColors(img);
        UnloadImage(img);
    }
    for (int i = 3; i <= 8; i++)
    {
        std::string path =
            "../assets/textures/wall" +
            std::to_string(i) +
            "_anim.png";

        wallAnim[i - 3] = LoadTexture(path.c_str());
    }
    floorimg = LoadImage("../assets/textures/floor.png");

}
void TextureManager::Unload()
{
    for (int i = 1; i < 8; i++)
    {
        if (wallPixels[i])
            UnloadImageColors(wallPixels[i]);

        if (walls[i].id != 0)
            UnloadTexture(walls[i]);
    }
}