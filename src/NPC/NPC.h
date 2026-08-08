#pragma once

#include "raylib.h"
#include <string>
#include <cmath>

class NPC
{
public:

    NPC();

    // Load NPC sprite sheet
    bool Load(
        const std::string& name,
        const std::string& texturePath,
        Vector2 position
    );

    void Unload();

    // Update NPC state
    void Update(Vector2 playerPos);

    // Draw NPC using raycasting projection + Z-buffer
    void Draw(
        Vector2 playerPos,
        Vector2 playerDir,
        Vector2 cameraPlane,
        const float Zbuffer[],
        int screenWidth,
        int screenHeight
    ) const;

    Vector2 GetPosition() const;

    float GetDepth(
        Vector2 playerPos,
        Vector2 playerDir
    ) const;

    bool IsNear() const;
    bool IsInteracting() const;

private:

    std::string name;

    Texture2D spriteSheet{};

    Vector2 position = { 0, 0 };

    // ------------------------------------------------
    // Animation
    // ------------------------------------------------

    int currentFrame = 0;
    int totalFrames = 4;

    float frameWidth = 0.0f;
    float frameHeight = 0.0f;

    float animationTimer = 0.0f;

    float idleFrameTime = 0.45f;

    // ------------------------------------------------
    // Interaction
    // ------------------------------------------------

    float interactionDistance = 2.0f;

    bool playerIsNear = false;
    bool interacting = false;
    bool isFountain = false;
    void SetFountain(bool value);
    float interactionTimer = 0.0f;

    float interactionDuration = 0.7f;

    void UpdateIdleAnimation(float dt);

    void StartInteraction();

    void DrawSprite(
        Vector2 playerPos,
        Vector2 playerDir,
        Vector2 cameraPlane,
        const float* zBuffer,
        int screenWidth,
        int screenHeight
    ) const;
};