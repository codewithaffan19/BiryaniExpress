#include "NPC.h"
#include<algorithm>
#include <cmath>
#include <string>

NPC::NPC()
{
}
void NPC::SetFountain(bool value)
{
    isFountain = value;
}
// ============================================================
// LOAD
// ============================================================

bool NPC::Load(
    const std::string& npcName,
    const std::string& texturePath,
    Vector2 npcPosition)
{
    name = npcName;
    position = npcPosition;

    // Automatically identify fountain
    isFountain = (name == "Fountain");

    spriteSheet = LoadTexture(texturePath.c_str());

    if (spriteSheet.id == 0)
    {
        return false;
    }

    // We expect one horizontal sprite sheet:
    //
    // | FRAME 0 | FRAME 1 | FRAME 2 | FRAME 3 |
    //

    frameWidth =
        (float)spriteSheet.width / totalFrames;

    frameHeight =
        (float)spriteSheet.height;

    SetTextureFilter(
        spriteSheet,
        TEXTURE_FILTER_POINT
    );

    currentFrame = 0;
    animationTimer = 0.0f;

    playerIsNear = false;
    interacting = false;
    interactionTimer = 0.0f;

    return true;
}

// ============================================================
// UNLOAD
// ============================================================

void NPC::Unload()
{
    if (spriteSheet.id != 0)
    {
        UnloadTexture(spriteSheet);
        spriteSheet.id = 0;
    }
}

// ============================================================
// UPDATE
// ============================================================

void NPC::Update(Vector2 playerPos)
{
    float dt = GetFrameTime();

    // --------------------------------------------------------
    // Calculate distance from player
    // --------------------------------------------------------

    float dx =
        position.x - playerPos.x;

    float dy =
        position.y - playerPos.y;

    float distance =
        sqrtf(dx * dx + dy * dy);

    playerIsNear =
        distance <= interactionDistance;

    // --------------------------------------------------------
    // INTERACTION ANIMATION
    // --------------------------------------------------------

    if (interacting)
    {
        interactionTimer += dt;

        // Frame 3 = interaction animation
        currentFrame = 3;

        if (interactionTimer >= interactionDuration)
        {
            interacting = false;
            interactionTimer = 0.0f;
            animationTimer = 0.0f;
            currentFrame = 0;
        }

        return;
    }

    // --------------------------------------------------------
    // PLAYER IS NEAR
    // --------------------------------------------------------

    if (playerIsNear)
    {
        // Frame 2 = player nearby
        currentFrame = 2;

        // Press E to interact
        if (IsKeyPressed(KEY_E))
        {
            StartInteraction();
        }

        return;
    }

    // --------------------------------------------------------
    // NORMAL IDLE ANIMATION
    // --------------------------------------------------------

    UpdateIdleAnimation(dt);
}

// ============================================================
// IDLE ANIMATION
// ============================================================

void NPC::UpdateIdleAnimation(float dt)
{
    animationTimer += dt;

    if (animationTimer >= idleFrameTime)
    {
        animationTimer = 0.0f;

        if (currentFrame == 0)
            currentFrame = 1;
        else
            currentFrame = 0;
    }
}

// ============================================================
// START INTERACTION
// ============================================================

void NPC::StartInteraction()
{
    interacting = true;

    interactionTimer = 0.0f;

    // Frame 3
    currentFrame = 3;
}

// ============================================================
// POSITION
// ============================================================

Vector2 NPC::GetPosition() const
{
    return position;
}

// ============================================================
// NEAR?
// ============================================================

bool NPC::IsNear() const
{
    return playerIsNear;
}

// ============================================================
// INTERACTING?
// ============================================================

bool NPC::IsInteracting() const
{
    return interacting;
}

// ============================================================
// DEPTH
// ============================================================

float NPC::GetDepth(
    Vector2 playerPos,
    Vector2 playerDir)
    const
{
    Vector2 sprite =
    {
        position.x - playerPos.x,
        position.y - playerPos.y
    };

    return
        sprite.x * playerDir.x +
        sprite.y * playerDir.y;
}

// ============================================================
// DRAW
// ============================================================

void NPC::Draw(
    Vector2 playerPos,
    Vector2 playerDir,
    Vector2 cameraPlane,
    const float* zBuffer,
    int screenWidth,
    int screenHeight
) const
{
    if (spriteSheet.id == 0)
        return;

    DrawSprite(
        playerPos,
        playerDir,
        cameraPlane,
        zBuffer,
        screenWidth,
        screenHeight
    );
}

// ============================================================
// DRAW SPRITE
// ============================================================

void NPC::DrawSprite(
    Vector2 playerPos,
    Vector2 playerDir,
    Vector2 cameraPlane,
    const float* zBuffer,
    int screenWidth,
    int screenHeight) const
{
    // --------------------------------------------------------
    // Position relative to player
    // --------------------------------------------------------

    Vector2 sprite =
    {
        position.x - playerPos.x,
        position.y - playerPos.y
    };

    // --------------------------------------------------------
    // Camera transformation
    // --------------------------------------------------------

    float invDet =
        1.0f /
        (
            cameraPlane.x * playerDir.y -
            cameraPlane.y * playerDir.x
            );

    float transformX =
        invDet *
        (
            playerDir.y * sprite.x -
            playerDir.x * sprite.y
            );

    float transformY =
        invDet *
        (
            -cameraPlane.y * sprite.x +
            cameraPlane.x * sprite.y
            );

    // Behind player
    if (transformY <= 0.1f)
        return;

    // --------------------------------------------------------
    // Screen position
    // --------------------------------------------------------

    int spriteScreenX =
        (int)(
            (screenWidth / 2.0f) *
            (1.0f + transformX / transformY)
            );

    // --------------------------------------------------------
    // Sprite size
    // --------------------------------------------------------

    float spriteScale =
        isFountain ? 0.30f : 0.55f;

    int spriteHeight =
        abs(
            (int)(
                (screenHeight / transformY) * spriteScale
                )
        );

    int spriteWidth =
        spriteHeight;

    // --------------------------------------------------------
    // Vertical position
    // --------------------------------------------------------

    int verticalOffset =
        (int)(spriteHeight * 0.25f);

    int drawStartY =
        -spriteHeight / 2 +
        screenHeight / 2 +
        verticalOffset;

    int drawEndY =
        spriteHeight / 2 +
        screenHeight / 2 +
        verticalOffset;


    // --------------------------------------------------------
    // Horizontal position
    // --------------------------------------------------------

    int drawStartX =
        -spriteWidth / 2 +
        spriteScreenX;

    int drawEndX =
        spriteWidth / 2 +
        spriteScreenX;

    if (drawStartX < 0)
        drawStartX = 0;

    if (drawEndX >= screenWidth)
        drawEndX = screenWidth - 1;

    // --------------------------------------------------------
    // Current animation frame
    // --------------------------------------------------------

    float frameOffsetX =
        currentFrame * frameWidth;

    // --------------------------------------------------------
    // Draw vertical stripes
    // --------------------------------------------------------

    for (int stripe = drawStartX;
        stripe < drawEndX;
        stripe++)
    {
        if (stripe < 0 ||
            stripe >= screenWidth)
            continue;

        // ----------------------------------------------------
        // Z-BUFFER
        // ----------------------------------------------------

        if (transformY >= zBuffer[stripe])
            continue;

        int trueStartX =
            -spriteWidth / 2 +
            spriteScreenX;

        int texX =
            (int)(
                (stripe - trueStartX) *
                frameWidth /
                spriteWidth
                );

        if (texX < 0)
            texX = 0;

        if (texX >= (int)frameWidth)
            texX = (int)frameWidth - 1;

        // ----------------------------------------------------
        // Source frame
        // ----------------------------------------------------

        Rectangle source =
        {
            frameOffsetX + (float)texX,
            0.0f,
            1.0f,
            frameHeight
        };

        // ----------------------------------------------------
        // Destination
        // ----------------------------------------------------

        Rectangle destination =
        {
            (float)stripe,
            (float)drawStartY,
            1.0f,
            (float)spriteHeight
        };

        DrawTexturePro(
            spriteSheet,
            source,
            destination,
            { 0, 0 },
            0.0f,
            WHITE
        );
    }
}