#pragma once

#include "raylib.h"
#include <string>
#include <cmath>
#include "../Player/Player.h"
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
    void Update(Vector2 playerPos,Player&p);

    // Draw NPC
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
    float radius;
    int myItem;
private:

    std::string name;

    Texture2D spriteSheet{};

    Vector2 position = { 0, 0 };

    // ========================================================
    // NPC TYPE
    // ========================================================

    bool isJack = false;

    // ========================================================
    // NORMAL NPC ANIMATION
    // ========================================================

    int currentFrame = 0;
    int totalFrames = 4;

    float frameWidth = 0.0f;
    float frameHeight = 0.0f;

    float animationTimer = 0.0f;

    float idleFrameTime = 0.45f;

    // ========================================================
    // JACK ANIMATION
    // ========================================================

    int jackFrame = 0;

    int jackInteractionStage = 0;

    bool jackAnimating = false;

    float jackAnimationTimer = 0.0f;

    float jackFrameTime = 1.0f;

    // ========================================================
    // JACK AUDIO
    // ========================================================

    Sound jackDialogue1{};
    Sound jackDialogue2{};
    Sound jackDialogue3{};

    bool jackAudioLoaded = false;

    // ========================================================
    // INTERACTION
    // ========================================================

    float interactionDistance = 1.5f;

    bool playerIsNear = false;
    bool interacting = false;

    float interactionTimer = 0.0f;

    float interactionDuration = 1.5f;

    // ========================================================
    // NORMAL NPC FUNCTIONS
    // ========================================================

    void UpdateIdleAnimation(float dt);

    void StartInteraction();

    // ========================================================
    // JACK FUNCTIONS
    // ========================================================

    void UpdateJack(float dt);

    void StartJackInteraction();

    // ========================================================
    // DRAW
    // ========================================================

    void DrawSprite(
        Vector2 playerPos,
        Vector2 playerDir,
        Vector2 cameraPlane,
        const float* zBuffer,
        int screenWidth,
        int screenHeight
    ) const;
};