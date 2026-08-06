#pragma once

#include "raylib.h"
#include "../Renderer/TextureManager.h"
class Map;

class Editor
{
public:
    Editor();

    void Update(Map& map);
    void Draw(Map& map);
    TextureManager* textures = nullptr;
    void SetTextureManager(TextureManager* tex)
    {
        textures = tex;
    }
private:

    Vector2 cameraOffset = { 0,0 };

    float tileSize = 40.0f;

    int currentTile = 1;

    void SaveShortcut(Map& map);
    static constexpr int SIDEBAR_WIDTH = 220;
    static constexpr int TILE_SIZE = 40;
    int selectedTile = 0;
    float scroll = 0;
    
};