#pragma once

#include <vector>

#include "raylib.h"
#include "../Renderer/TextureManager.h"

class Map;

class Editor
{
public:

    Editor();

    // =========================================================
    // MAIN EDITOR
    // =========================================================

    void Update(Map& map);
    void Draw(Map& map);

    // =========================================================
    // TEXTURE MANAGER
    // =========================================================

    TextureManager* textures = nullptr;

    void SetTextureManager(TextureManager* tex)
    {
        textures = tex;
    }
    // =========================================================
  // CAMERA / EDITOR SETTINGS
  // =========================================================

    Vector2 cameraOffset = { 0, 0 };

    float tileSize = 40.0f;

    int currentTile = 1;

    // =========================================================
    // SIDEBAR
    // =========================================================

    static constexpr int SIDEBAR_WIDTH = 220;
    static constexpr int TILE_SIZE = 40;

    int selectedTile = 0;

    int scrollOffset = 0;

    static constexpr int VISIBLE_ITEMS = 18;
private:

    // =========================================================
    // PARTITION
    // =========================================================

    TextureManager::Partition currentPartition =
        TextureManager::Partition::Street;

  

    // =========================================================
    // PARTITION UI
    // =========================================================

    void HandlePartitionSelection(Vector2 mouse);

    void DrawPartitionTabs();

    // =========================================================
    // SAVE
    // =========================================================

    void SaveShortcut(Map& map);
};