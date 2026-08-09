#include "NPC.h"

#include "raylib.h"
#include <cmath>

// ============================================================
// CONSTRUCTOR
// ============================================================

NPC::NPC()
{
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

    // --------------------------------------------------------
    // Identify Jack
    // --------------------------------------------------------

    isJack =
        (name == "Jack" ||
            name == "JACK");
    // ========================================================
// JACK AUDIO
// ========================================================

    if (isJack)
    {
        jackDialogue1 =
            LoadSound("../assets/audio/jack_dialogue_1.wav");

        jackDialogue2 =
            LoadSound("../assets/audio/jack_dialogue_2.wav");

        jackDialogue3 =
            LoadSound("../assets/audio/jack_dialogue_3.wav");

        jackAudioLoaded =
            (jackDialogue1.frameCount > 0 &&
                jackDialogue2.frameCount > 0 &&
                jackDialogue3.frameCount > 0);
    }
    // --------------------------------------------------------
    // Load sprite sheet
    // --------------------------------------------------------

    spriteSheet =
        LoadTexture(texturePath.c_str());

    if (spriteSheet.id == 0)
    {
        return false;
    }

    SetTextureFilter(
        spriteSheet,
        TEXTURE_FILTER_POINT
    );

    // --------------------------------------------------------
    // NORMAL NPCS
    // 1 row x 4 columns
    // --------------------------------------------------------

    if (!isJack)
    {
        totalFrames = 4;

        frameWidth =
            (float)spriteSheet.width / 4.0f;

        frameHeight =
            (float)spriteSheet.height;
    }

    // --------------------------------------------------------
    // JACK
    // 4 columns x 2 rows
    // --------------------------------------------------------

    else
    {
        totalFrames = 8;

        frameWidth =
            (float)spriteSheet.width / 4.0f;

        frameHeight =
            (float)spriteSheet.height / 2.0f;
    }

    // --------------------------------------------------------
    // Reset animation
    // --------------------------------------------------------

    currentFrame = 0;

    animationTimer = 0.0f;

    playerIsNear = false;
    interacting = false;

    interactionTimer = 0.0f;

    // Jack reset
    jackFrame = 0;
    jackInteractionStage = 0;
    jackAnimating = false;
    jackAnimationTimer = 0.0f;

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

    // ========================================================
    // JACK AUDIO
    // ========================================================

    if (isJack && jackAudioLoaded)
    {
        UnloadSound(jackDialogue1);
        UnloadSound(jackDialogue2);
        UnloadSound(jackDialogue3);

        jackAudioLoaded = false;
    }
}

// ============================================================
// UPDATE
// ============================================================

void NPC::Update(Vector2 playerPos)
{
    float dt = GetFrameTime();

    // --------------------------------------------------------
    // Distance from player
    // --------------------------------------------------------

    float dx =
        position.x - playerPos.x;

    float dy =
        position.y - playerPos.y;

    float distance =
        sqrtf(dx * dx + dy * dy);

    playerIsNear =
        distance <= interactionDistance;

    // ========================================================
    // JACK
    // ========================================================

    if (isJack)
    {
        UpdateJack(dt);
        return;
    }

    // ========================================================
    // NORMAL NPCS
    // ========================================================

    // --------------------------------------------------------
    // Currently doing normal E interaction
    // --------------------------------------------------------

    if (interacting)
    {
        interactionTimer += dt;

        // Frame 4
        currentFrame = 3;

        if (interactionTimer >= interactionDuration)
        {
            interacting = false;

            interactionTimer = 0.0f;

            // Go back to frame 3 while player remains near
            if (playerIsNear)
            {
                currentFrame = 2;
            }
            else
            {
                currentFrame = 0;
                animationTimer = 0.0f;
            }
        }

        return;
    }

    // --------------------------------------------------------
    // Player near normal NPC
    // --------------------------------------------------------

    if (playerIsNear)
    {
        // Frame 3 while player is near
        currentFrame = 2;

        // Press E
        if (IsKeyPressed(KEY_E))
        {
            StartInteraction();
        }

        return;
    }

    // --------------------------------------------------------
    // Player far away
    // --------------------------------------------------------

    UpdateIdleAnimation(dt);
}

// ============================================================
// NORMAL NPC IDLE ANIMATION
// ============================================================

void NPC::UpdateIdleAnimation(float dt)
{
    animationTimer += dt;

    if (animationTimer >= idleFrameTime)
    {
        animationTimer = 0.0f;

        // Frames 1 and 2
        // Internally they are 0 and 1.
        if (currentFrame == 0)
        {
            currentFrame = 1;
        }
        else
        {
            currentFrame = 0;
        }
    }
}

// ============================================================
// NORMAL NPC INTERACTION
// ============================================================

void NPC::StartInteraction()
{
    interacting = true;

    interactionTimer = 0.0f;

    // Frame 4
    currentFrame = 3;
}

// ============================================================
// JACK UPDATE
// ============================================================

// ============================================================
// JACK UPDATE
// ============================================================

void NPC::UpdateJack(float dt)
{
    // --------------------------------------------------------
    // PLAYER IS FAR AWAY
    // --------------------------------------------------------

    if (!playerIsNear)
    {
        // Completely reset Jack when player walks away
        jackFrame = 0;
        jackInteractionStage = 0;
        jackAnimating = false;
        jackAnimationTimer = 0.0f;

        return;
    }

    // --------------------------------------------------------
    // JACK IS CURRENTLY ANIMATING
    // --------------------------------------------------------

    if (jackAnimating)
    {
        jackAnimationTimer += dt;

        if (jackAnimationTimer >= jackFrameTime)
        {
            jackAnimationTimer = 0.0f;

            // =================================================
            // SECOND E ANIMATION
            //
            // Frame sequence:
            //
            // 2 -> 3 -> 4
            //
            // Row 1 image 3
            // Row 1 image 4
            // Row 2 image 1
            //
            // STOP at frame 4.
            // DO NOT continue to frame 5.
            // =================================================

            if (jackInteractionStage == 2)
            {
                jackFrame++;

                if (jackFrame > 4)
                {
                    // Stay on Row 2 Image 1
                    jackFrame = 4;

                    // STOP animation
                    jackAnimating = false;

                    jackAnimationTimer = 0.0f;
                }

                return;
            }

            // =================================================
            // THIRD E ANIMATION
            //
            // Frame sequence:
            //
            // 5 -> 6 -> 7
            //
            // Row 2 image 2
            // Row 2 image 3
            // Row 2 image 4
            //
            // After frame 7:
            // RESET TO FRAME 0.
            // =================================================

            if (jackInteractionStage == 3)
            {
                jackFrame++;

                if (jackFrame >= 8)
                {
                    // Reset Jack completely
                    jackFrame = 0;

                    jackInteractionStage = 0;

                    jackAnimating = false;

                    jackAnimationTimer = 0.0f;
                }

                return;
            }
        }

        return;
    }

    // --------------------------------------------------------
    // WAITING FOR E
    // --------------------------------------------------------

    if (IsKeyPressed(KEY_E))
    {
        StartJackInteraction();
    }
}

// ============================================================
// JACK INTERACTION
// ============================================================

void NPC::StartJackInteraction()
{
    // ========================================================
    // FIRST E
    //
    // Frame 1 -> Frame 2
    //
    // Internal:
    // 0 -> 1
    // ========================================================

    if (jackInteractionStage == 0)
    {
        jackFrame = 1;

        jackInteractionStage = 1;

        jackAnimating = false;

        jackAnimationTimer = 0.0f;

        // Play dialogue 1
        if (jackAudioLoaded)
        {
            PlaySound(jackDialogue1);
        }

        return;
    }

    // ========================================================
    // SECOND E
    //
    // Row 1:
    //
    // Frame 3
    // Frame 4
    //
    // Row 2:
    //
    // Frame 1
    //
    // Internal:
    // 2 -> 3 -> 4
    // ========================================================

    if (jackInteractionStage == 1)
    {
        jackFrame = 2;

        jackInteractionStage = 2;

        jackAnimating = true;

        jackAnimationTimer = 0.0f;

        // Play dialogue 2
        if (jackAudioLoaded)
        {
            PlaySound(jackDialogue2);
        }

        return;
    }

    // ========================================================
    // THIRD E
    //
    // Row 2:
    //
    // Frame 2
    // Frame 3
    // Frame 4
    //
    // Internal:
    // 5 -> 6 -> 7
    //
    // After frame 7:
    // RESET TO FRAME 0
    // ========================================================

    if (jackInteractionStage == 2)
    {
        jackFrame = 5;

        jackInteractionStage = 3;

        jackAnimating = true;

        jackAnimationTimer = 0.0f;

        // Play dialogue 3
        if (jackAudioLoaded)
        {
            PlaySound(jackDialogue3);
        }

        return;
    }

    // ========================================================
    // SAFETY RESET
    // ========================================================

    jackFrame = 0;

    jackInteractionStage = 0;

    jackAnimating = false;

    jackAnimationTimer = 0.0f;
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
    if (isJack)
        return jackAnimating;

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
    const float Zbuffer[],
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
        Zbuffer,
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

    // ========================================================
    // SPRITE SIZE
    // ========================================================

    float spriteScale = 0.55f;

    int spriteHeight =
        abs(
            (int)(
                (screenHeight / transformY) *
                spriteScale
                )
        );

    // Keep original proportions
    float aspectRatio =
        frameWidth / frameHeight;

    int spriteWidth =
        (int)(spriteHeight * aspectRatio);

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

    if (drawStartY < 0)
        drawStartY = 0;

    if (drawEndY >= screenHeight)
        drawEndY = screenHeight - 1;

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

    // Completely outside screen
    if (drawStartX >= screenWidth ||
        drawEndX < 0)
    {
        return;
    }

    // ========================================================
    // SELECT FRAME
    // ========================================================

    int frameToDraw;

    if (isJack)
    {
        // Jack uses 0-7
        frameToDraw = jackFrame;
    }
    else
    {
        // Normal NPC uses 0-3
        frameToDraw = currentFrame;
    }

    // --------------------------------------------------------
    // Frame position
    // --------------------------------------------------------

    float frameOffsetX =
        (frameToDraw % 4) *
        frameWidth;

    float frameOffsetY = 0.0f;

    // Jack has two rows
    if (isJack)
    {
        frameOffsetY =
            (frameToDraw / 4) *
            frameHeight;
    }

    // ========================================================
    // DRAW STRIPE BY STRIPE
    // ========================================================

    for (int stripe = drawStartX;
        stripe < drawEndX;
        stripe++)
    {
        if (stripe < 0 ||
            stripe >= screenWidth)
        {
            continue;
        }

        // ----------------------------------------------------
        // Z BUFFER
        // ----------------------------------------------------

        if (transformY >= zBuffer[stripe])
            continue;

        // ----------------------------------------------------
        // Texture X
        // ----------------------------------------------------

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
        // Source rectangle
        // ----------------------------------------------------

        Rectangle source =
        {
            frameOffsetX + (float)texX,
            frameOffsetY,
            1.0f,
            frameHeight
        };

        // ----------------------------------------------------
        // Destination rectangle
        // ----------------------------------------------------

        Rectangle destination =
        {
            (float)stripe,
            (float)drawStartY,
            1.0f,
            (float)(drawEndY - drawStartY)
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