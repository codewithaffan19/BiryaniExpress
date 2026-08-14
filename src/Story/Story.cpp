#include "Story.h"

#include "raylib.h"
#include "raymath.h"

#include "../World/Collision.h"
#include <cmath>
#include <iostream>
// ============================================================
// CONSTRUCTOR
// ============================================================

Story::Story()
{
    Initialize();
}

// ============================================================
// INITIALIZE
// ============================================================

void Story::Initialize()
{
    // ========================================================
    // GUARD 1
    //
    // Row 15, Column 19
    // Carries key for Door 1
    // ========================================================

    guards[0].position =
    {
        18.5f,
        14.5f
    };

    guards[0].key = KEY_DOOR_1;
    guards[0].dialogues[0] = "Affan: Uncle, yahan se nikalne ka koi raasta hai? \n Guard: Raasta toh hai beta... lekin har raasta seedha exit tak nahi jata.";
    guards[0].dialogues[1] = "Affan: Toh mujhe sahi darwaze ki chabi de dein. \n Guard: Hahaha... sahi chabi aur sahi chaye har kisi ko nahi milti";
    guards[0].dialogues[2] = "Guard: Ye lo chabi... seedha aage jao aur jo darwaza mile, usay try karna.\n Aur haan... agar darwaza na khule, toh wapas mat aana!";
    // ========================================================
    // GUARD 2
    //
    // Row 15, Column 5
    // Carries key for Door 2
    // ========================================================

    guards[1].position =
    {
        4.5f,
        16.5f
    };

    guards[1].key = KEY_DOOR_3;
    guards[1].dialogues[0] = "Affan: Uncle, yahan se nikalne ki asli chaabi de dein, baar baar scam ho raha hai.\n Guard 2: Beta, de dete hain... bas paisa lagay ga.";
    guards[1].dialogues[1] = "Affan: Ye lein... bas chaabi sahi darwazay ki de dein.\n Guard 2: Hahaha... fikar na karo beta, dekhte hain.";
    guards[1].dialogues[2] = "Guard: Ye lo chaabi... seedha EXIT ke paas jao ge.";
    // ========================================================
    // GUARD 3
    //
    // Row 15, Column 11
    // Carries key for Door 3
    // ========================================================

    guards[2].position =
    {
        8.5f,
        15.5f
    };

    guards[2].key = KEY_DOOR_2;
    guards[2].dialogues[0] = "Guard 2: Bachay, is waqt bahir kyun ghoom rahay ho? Raat kaafi ho gayi hai.";
    guards[2].dialogues[1] = "Affan: Mujhe yahan se nikalna hai... bahar jaane walay darwazay ki chaabi de dein.\n Guard 2: Fikar na karo, acha bura waqt ata jata rehta h";
    guards[2].dialogues[2] = "Guard 2: Ye lo chaabi aur bhaag jao! Jaldi se nikalo...\n bahar jaa ke ayashi karna!";

    // ========================================================
// STORY DOORS
//
// Editor coordinates are 1-based:
//
// Door 1 = Row 22, Column 12
// Door 2 = Row 22, Column 15
// Door 3 = Row 23, Column 17
//
// World coordinates are column - 0.5,
// row - 0.5.
// ========================================================

    doorPositions[0] =
    {
        11.5f,
        21.5f
    };

    doorPositions[1] =
    {
        14.5f,
        21.5f
    };

    doorPositions[2] =
    {
        16.5f,
        21.5f
    };

    // ========================================================
    // TELEPORT DESTINATIONS
    //
    // Door 1:
    // Row 10, Column 24
    // ========================================================

    doorTeleportPositions[0] =
    {
        23.5f,
        10.5f
    };

    // ========================================================
    // Door 2
    //
    // No teleport.
    // Police event.
    // ========================================================

    doorTeleportPositions[1] =
    {
        0.0f,
        0.0f
    };

    // ========================================================
    // Door 3:
    //
    // Row 23, Column 16
    // ========================================================

    doorTeleportPositions[2] =
    {
        16.5f,
        23.5f
    };
    // ========================================================
// RESET STORY DOORS
// ========================================================

    for (int i = 0; i < 3; i++)
    {
        doorOpened[i] = false;
    }

    policeSpawnTriggered = false;
    // ========================================================
    // WIN POSITION
    // ========================================================

    winPosition =
    {
        23.5f,
        23.5f
    };

    // ========================================================
    // RESET STORY
    // ========================================================

    currentKey = KEY_NONE;

    playerLocked = false;

    won = false;

    dialogueBox = false;

    activeGuard = -1;
    screenMessage = "";

    screenMessageTimer = 0.0f;

    currentDoor = -1;
    // ========================================================
    // RESET GUARDS
    // ========================================================

    for (int i = 0; i < 3; i++)
    {
        guards[i].interactionStage = 0;
        guards[i].interacting = false;
        guards[i].interactionTimer = 0.0f;
        guards[i].frame = 0;
        guards[i].completed = false;
    }

    // ========================================================
    // LOAD GUARD TEXTURES
    // ========================================================

    LoadStoryTextures();

}
void Story::CheckGuradCollosion(Guard& G1, Player& p, Map& m1) {
    float dx = p.position.x - G1.position.x;
    float dy = p.position.y - G1.position.y;
    float dis = sqrt((dx * dx) + (dy * dy));
    float radiiSum = p.radius + G1.radius;
    if (dis > 0.001f&&dis<radiiSum) {
        float overlap = radiiSum - dis;
        float Push = overlap / 2.0f;
        float NormalX = dx / dis;
        float NormalY = dy / dis;
        Vector2 PlayerBumpVelocity = { -NormalX * Push, -NormalY * Push };

        Vector2 OldPlayerPos = G1.position;
        G1.position = CheckMapCollosion(G1.position, G1.radius, PlayerBumpVelocity, m1);

        float movedX = G1.position.x - OldPlayerPos.x;
        float movedY = G1.position.y - OldPlayerPos.y;
        float actualMovedDis = sqrt((movedX * movedX) + (movedY * movedY));
        //Moveable distance check
        float EnemyForce = Push + (Push - actualMovedDis);

        Vector2 EnemyBumpVelocity = { NormalX * EnemyForce, NormalY * EnemyForce };
        p.position = CheckMapCollosion(p.position, p.radius, EnemyBumpVelocity, m1);
    }
}

// ============================================================
// LOAD STORY TEXTURES
// ============================================================

void Story::LoadStoryTextures()
{
    // ========================================================
    // GUARD 1
    //
    // 5 horizontal frames
    // ========================================================

    guards[0].texture =
        LoadTexture(
            "../assets/textures/guard1.png"
        );

    if (guards[0].texture.id == 0)
    {
        TraceLog(
            LOG_ERROR,
            "Failed to load guard1.png"
        );
    }

    // ========================================================
    // GUARD 2
    // ========================================================

    guards[1].texture =
        LoadTexture(
            "../assets/textures/guard2.png"
        );

    if (guards[1].texture.id == 0)
    {
        TraceLog(
            LOG_ERROR,
            "Failed to load guard2.png"
        );
    }

    // ========================================================
    // GUARD 3
    // ========================================================

    guards[2].texture =
        LoadTexture(
            "../assets/textures/guard3.png"
        );

    if (guards[2].texture.id == 0)
    {
        TraceLog(
            LOG_ERROR,
            "Failed to load guard3.png"
        );
    }

    // ========================================================
    // FILTER GUARDS
    // ========================================================

    for (int i = 0; i < 3; i++)
    {
        if (guards[i].texture.id != 0)
        {
            SetTextureFilter(
                guards[i].texture,
                TEXTURE_FILTER_POINT
            );
        }
    }

    // ========================================================
    // ALL GUARDS HAVE 5 HORIZONTAL FRAMES
    //
    // [FRAME 1][FRAME 2][FRAME 3][FRAME 4][FRAME 5]
    // ========================================================

    if (guards[0].texture.id != 0)
    {
        guardFrameWidth =
            (float)guards[0].texture.width / 5.0f;

        guardFrameHeight =
            (float)guards[0].texture.height;
    }

}

// ============================================================
// UNLOAD
// ============================================================

void Story::Unload()
{
    // ========================================================
    // GUARDS
    // ========================================================

    for (int i = 0; i < 3; i++)
    {
        if (guards[i].texture.id != 0)
        {
            UnloadTexture(
                guards[i].texture
            );

            guards[i].texture.id = 0;
        }
    }

}

// ============================================================
// UPDATE
// ============================================================

void Story::Update(
    Player& player,
    bool insideNeon
)
{
    float dt = GetFrameTime();

    // ========================================================
    // UPDATE MESSAGE TIMER
    // ========================================================

    if (screenMessageTimer > 0.0f)
    {
        screenMessageTimer -= dt;

        if (screenMessageTimer < 0.0f)
            screenMessageTimer = 0.0f;
    }

    // ========================================================
    // STORY ONLY WORKS INSIDE NEON
    // ========================================================

    if (!insideNeon)
    {
        playerLocked = false;
        activeGuard = -1;
        dialogueBox = false;

        currentDoor = -1;

        return;
    }

    // ========================================================
    // PLAYER CURRENTLY TALKING TO GUARD
    // ========================================================

    if (playerLocked)
    {
        if (activeGuard >= 0 &&
            activeGuard < 3)
        {
            UpdateGuardInteraction(
                player,
                activeGuard
            );
        }

        return;
    }

    // ========================================================
    // CHECK GUARDS
    // ========================================================

    for (int i = 0; i < 3; i++)
    {
        // ----------------------------------------------------
        // Cannot talk to another guard while carrying key.
        // ----------------------------------------------------

        if (currentKey != KEY_NONE)
            break;

        // ----------------------------------------------------
        // Guard already used.
        // ----------------------------------------------------

        if (guards[i].completed)
            continue;

        // ----------------------------------------------------
        // Near guard?
        // ----------------------------------------------------
        CheckGuradCollosion(guards[i], player, m1);
        if (IsNear(
            player.GetPosition(),
            guards[i].position,
            interactionDistance))
        {
            if (IsKeyPressed(KEY_E))
            {
                activeGuard = i;

                StartGuardInteraction(i);

                return;
            }
        }

    }

    // ========================================================
    // CHECK THREE STORY DOORS
    // ========================================================

    currentDoor = -1;

    for (int i = 0; i < 3; i++)
    {
        // Door already opened?
        if (doorOpened[i])
            continue;

        // ----------------------------------------------------
        // Is player near this door?
        // ----------------------------------------------------

        if (IsNear(
            player.GetPosition(),
            doorPositions[i],
            2.0f))
        {
            currentDoor = i;

            // ------------------------------------------------
            // BEFORE E:
            //
            // Always show this while standing near door.
            // ------------------------------------------------

            if (screenMessageTimer <= 0.0f)
            {
                SetScreenMessage(
                    "Press E to insert key",
                    0.15f
                );
            }

            // ------------------------------------------------
            // E PRESSED
            // ------------------------------------------------

            if (IsKeyPressed(KEY_E))
            {
                int doorNumber =
                    i + 1;

                // --------------------------------------------
                // TRY TO OPEN DOOR
                // --------------------------------------------

                if (TryUseDoor(doorNumber))
                {
                    // ----------------------------------------
                    // DOOR 1
                    // ----------------------------------------

                    if (doorNumber == 1)
                    {
                        player.position =
                            GetTeleportPosition(1);
                    }

                    // ----------------------------------------
                    // DOOR 2
                    //
                    // No teleport.
                    // Police event only.
                    // ----------------------------------------

                    else if (doorNumber == 2)
                    {
                        // Police event already triggered
                        // inside TryUseDoor().
                        player.isPoliceTriggered = true;
                    }

                    // ----------------------------------------
                    // DOOR 3
                    // ----------------------------------------

                    else if (doorNumber == 3)
                    {
                        player.position =
                            GetTeleportPosition(3);
                    }
                }
            }

            break;
        }
    }

    // ========================================================
    // WIN LOCATION
    //
    // Row 24, Column 24
    // ========================================================

    if (IsNear(
        player.GetPosition(),
        winPosition,
        1.2f))
    {
        if (!won)
        {
            won = true;

            SetScreenMessage(
                "YOU WIN!",
                5.0f
            );

            std::cout
                << "=================================\n";

            std::cout
                << "YOU WIN!\n";

            std::cout
                << "=================================\n";
        }
    }
}

// ============================================================
// START GUARD INTERACTION
// ============================================================

void Story::StartGuardInteraction(
    int guardIndex
)
{
    if (guardIndex < 0 ||
        guardIndex >= 3)
    {
        return;
    }

    Guard& guard =
        guards[guardIndex];

    // ========================================================
    // SAFETY
    // ========================================================

    if (guard.completed)
        return;

    // ========================================================
    // FIRST E
    //
    // Frame 1 -> Frame 2
    // ========================================================

    if (guard.interactionStage == 0)
    {
        playerLocked = true;

        activeGuard = guardIndex;

        guard.frame = 1;

        guard.interactionStage = 1;

        guard.interacting = false;

        guard.interactionTimer = 0.0f;

        dialogueBox = true;

        std::cout
            << "Guard interaction started.\n";

        return;
    }

    // ========================================================
    // SECOND E
    // Start looping:
 
    // Frame 2 -> Frame 3 -> Frame 4 -> Frame 2...
    //
    // This continues until the THIRD E.
    // ========================================================

    if (guard.interactionStage == 1)
    {
        playerLocked = true;

        activeGuard = guardIndex;

        guard.frame = 1;

        guard.interactionStage = 2;

        guard.interacting = true;

        guard.interactionTimer = 0.0f;

        dialogueBox = true;

        std::cout
            << "Guard animation started.\n";

        return;
    }

    // ========================================================
    // THIRD E
    //
    // Frame 5
    // Give key
    // Stay for 2 seconds
    // Then return to frame 1.
    //
    // Guard becomes permanently completed.
    // ========================================================

    if (guard.interactionStage == 2)
    {
        guard.frame = 4;

        guard.interactionStage = 3;

        guard.interacting = true;

        guard.interactionTimer = 0.0f;

        dialogueBox = true;

        GiveKey(guardIndex);

        std::cout
            << "Guard finished interaction.\n";
        
        return;
    }

}

// ============================================================
// UPDATE GUARD INTERACTION
// ============================================================

void Story::UpdateGuardInteraction(
    Player& player,
    int guardIndex
)
{
    (void)player;

    if (guardIndex < 0 ||
        guardIndex >= 3)
    {
        playerLocked = false;
        activeGuard = -1;

        return;
    }

    Guard& guard =
        guards[guardIndex];

    float dt =
        GetFrameTime();

    // ========================================================
    // THIRD STAGE
    //
    // Frame 5 is displayed for 2 seconds.
    // ========================================================

    if (guard.interactionStage == 3)
    {
        guard.interactionTimer += dt;

        if (guard.interactionTimer >= 2.0f)
        {
            guard.frame = 0;

            guard.interactionStage = 4;

            guard.interacting = false;

            guard.completed = true;

            guard.interactionTimer = 0.0f;

            dialogueBox = false;

            playerLocked = false;

            activeGuard = -1;

            std::cout
                << "Guard interaction permanently completed.\n";
        }

        return;
    }

    // ========================================================
    // SECOND STAGE
    //
    // Loop:
    //
    // Frame 2 -> Frame 3 -> Frame 4 -> Frame 2...
    //
    // Every 0.20 seconds.
    // ========================================================

    if (guard.interactionStage == 2)
    {
        guard.interactionTimer += dt;

        const float animationSpeed = 0.40f;

        if (guard.interactionTimer >= animationSpeed)
        {
            guard.interactionTimer = 0.0f;

            // Frame indexes:
            //
            // 1 = Frame 2
            // 2 = Frame 3
            // 3 = Frame 4

            guard.frame++;

            if (guard.frame > 3)
            {
                guard.frame = 1;
            }
        }

        // ----------------------------------------------------
        // THIRD E
        // ----------------------------------------------------

        if (IsKeyPressed(KEY_E))
        {
            StartGuardInteraction(
                guardIndex
            );
        }
        
        return;
    }

    // ========================================================
    // FIRST STAGE
    //
    // Waiting for second E.
    // ========================================================

    if (guard.interactionStage == 1)
    {
        if (IsKeyPressed(KEY_E))
        {
            StartGuardInteraction(
                guardIndex
            );
        }

        return;
    }

}

// ============================================================
// GIVE KEY
// ============================================================

void Story::GiveKey(int guardIndex)
{
    if (guardIndex < 0 ||
        guardIndex >= 3)
    {
        return;
    }

    // ========================================================
    // ALREADY HAVE KEY
    // ========================================================

    if (currentKey != KEY_NONE)
    {
        SetScreenMessage(
            "You are already carrying a key!",
            2.0f
        );

        std::cout
            << "You are already carrying a key.\n";

        return;
    }

    // ========================================================
    // COLLECT KEY
    // ========================================================

    currentKey =
        guards[guardIndex].key;

    PrintKeyName(currentKey);

    SetScreenMessage(
        "Key collected!",
        2.5f
    );

    std::cout
        << "Key collected.\n";
}
// ============================================================
// HAS ANY KEY
// ============================================================

bool Story::HasAnyKey() const
{
    return currentKey != KEY_NONE;
}
// ============================================================
// SCREEN MESSAGE
// ============================================================

void Story::SetScreenMessage(
    const std::string& message,
    float duration
)
{
    screenMessage = message;
    screenMessageTimer = duration;
}
// ============================================================
// IS DOOR OPENED
// ============================================================

bool Story::IsDoorOpened(int doorNumber) const
{
    if (doorNumber < 1 ||
        doorNumber > 3)
    {
        return false;
    }

    return doorOpened[doorNumber - 1];
}

// ============================================================
// TRY USE STORY DOOR
//
// Door 1 -> Key 1 -> teleport
// Door 2 -> Key 2 -> police event
// Door 3 -> Key 3 -> teleport
//
// Once a door is opened, it can NEVER be used again.
// ============================================================

bool Story::TryUseDoor(int doorNumber)
{
    // ========================================================
    // VALID DOOR?
    // ========================================================

    if (doorNumber < 1 ||
        doorNumber > 3)
    {
        return false;
    }

    int index =
        doorNumber - 1;

    // ========================================================
    // ALREADY OPENED
    // ========================================================

    if (doorOpened[index])
    {
        SetScreenMessage(
            "Door already opened!",
            2.0f
        );

        return false;
    }

    // ========================================================
    // REQUIRED KEY
    // ========================================================

    StoryKey requiredKey =
        KEY_NONE;

    switch (doorNumber)
    {
    case 1:
        requiredKey = KEY_DOOR_1;
        break;

    case 2:
        requiredKey = KEY_DOOR_2;
        break;

    case 3:
        requiredKey = KEY_DOOR_3;
        break;
    }

    // ========================================================
    // NO KEY AT ALL
    // ========================================================

    if (currentKey == KEY_NONE)
    {
        SetScreenMessage(
            "Collect key first!",
            2.0f
        );

        std::cout
            << "Collect key first!\n";

        return false;
    }

    // ========================================================
    // WRONG KEY
    // ========================================================

    if (currentKey != requiredKey)
    {
        SetScreenMessage(
            "Key is not for this door!",
            2.0f
        );

        std::cout
            << "Key is not for this door!\n";

        return false;
    }

    // ========================================================
    // CORRECT KEY
    // ========================================================

    currentKey = KEY_NONE;

    doorOpened[index] = true;

    // ========================================================
    // DOOR 1
    // ========================================================

    if (doorNumber == 1)
    {
        SetScreenMessage(
            "Door 1 opens!",
            2.0f
        );

        std::cout
            << "DOOR 1 OPENED\n";

        std::cout
            << "Teleporting to Row 10 Column 24.\n";

        return true;
    }

    // ========================================================
    // DOOR 2
    // ========================================================

    if (doorNumber == 2)
    {
        policeSpawnTriggered = true;

        SetScreenMessage(
            "Door 2 opens! Police spawned!",
            3.0f
        );

        std::cout
            << "DOOR 2 OPENED\n";

        std::cout
            << "POLICE SPAWN EVENT TRIGGERED!\n";

        return true;
    }

    // ========================================================
    // DOOR 3
    // ========================================================

    if (doorNumber == 3)
    {
        SetScreenMessage(
            "Door 3 opens!",
            2.0f
        );

        std::cout
            << "DOOR 3 OPENED\n";

        std::cout
            << "Teleporting to Row 23 Column 16.\n";

        return true;
    }

    return false;
}
// ============================================================
// GET TELEPORT POSITION
// ============================================================

Vector2 Story::GetTeleportPosition(int doorNumber) const
{
    if (doorNumber < 1 ||
        doorNumber > 3)
    {
        return {};
    }

    return doorTeleportPositions[doorNumber - 1];
}
// ============================================================
// POLICE EVENT
// ============================================================

bool Story::WasPoliceSpawnTriggered() const
{
    return policeSpawnTriggered;
}

// ============================================================
// PRINT KEY
// ============================================================

void Story::PrintKeyName(
    StoryKey key
) const
{
    switch (key)
    {
    case KEY_DOOR_1:

        std::cout
            << "Key 1 collected.\n";

        break;

    case KEY_DOOR_2:

        std::cout
            << "Key 2 collected.\n";

        break;

    case KEY_DOOR_3:

        std::cout
            << "Key 3 collected.\n";

        break;

    default:

        break;
    }

}

// ============================================================
// HAS KEY FOR TILE DOOR
// ============================================================
//
// doorNumber:
// 1 = Door 1
// 2 = Door 2
// 3 = Door 3
//
// The actual tile door remains outside Story.
// Story only checks whether the player has the required key.
// ============================================================

bool Story::HasKeyForDoor(
    int doorNumber
) const
{
    StoryKey requiredKey = KEY_NONE;

    switch (doorNumber)
    {
    case 1:
        requiredKey = KEY_DOOR_1;
        break;

    case 2:
        requiredKey = KEY_DOOR_2;
        break;

    case 3:
        requiredKey = KEY_DOOR_3;
        break;

    default:
        return false;
    }

    return currentKey == requiredKey;

}



// ============================================================
// DISTANCE CHECK
// ============================================================

bool Story::IsNear(
    Vector2 a,
    Vector2 b,
    float distance
) const
{
    float dx =
        a.x - b.x;

    float dy =
        a.y - b.y;

    return sqrtf(
        dx * dx +
        dy * dy
    ) <= distance;

}

// ============================================================
// PLAYER LOCKED
// ============================================================

bool Story::IsPlayerLocked() const
{
    return playerLocked;
}

// ============================================================
// WON
// ============================================================

bool Story::IsWon() const
{
    return won;
}

// ============================================================
// DRAW UI
// ============================================================

void Story::DrawUI() const
{
    // ========================================================
    // STORY MESSAGE
    // ========================================================

    if (screenMessageTimer > 0.0f &&
        !screenMessage.empty())
    {
        int fontSize = 28;

        int textWidth =
            MeasureText(
                screenMessage.c_str(),
                fontSize
            );

        int x =
            GetScreenWidth() / 2 -
            textWidth / 2;

        int y =
            GetScreenHeight() - 90;

        // Background
        DrawRectangle(
            x - 20,
            y - 10,
            textWidth + 40,
            50,
            Fade(BLACK, 0.75f)
        );

        // Border
        DrawRectangleLines(
            x - 20,
            y - 10,
            textWidth + 40,
            50,
            WHITE
        );

        // Text
        DrawText(
            screenMessage.c_str(),
            x,
            y,
            fontSize,
            YELLOW
        );
    }

    // ========================================================
    // GUARD DIALOGUE BOX
    // ========================================================

    if (playerLocked)
    {
        DrawRectangle(
            100,
            GetScreenHeight() - 180,
            GetScreenWidth() - 200,
            120,
            Fade(BLACK, 0.75f)
        );

        DrawRectangleLines(
            100,
            GetScreenHeight() - 180,
            GetScreenWidth() - 200,
            120,
            WHITE
        );

        if (activeGuard >= 0 && activeGuard < 3)
        {
            int dialogueIndex =
                guards[activeGuard].interactionStage - 1;

            if (dialogueIndex < 0)
                dialogueIndex = 0;

            if (dialogueIndex > 2)
                dialogueIndex = 2;

            DrawText(
                guards[activeGuard]
                .dialogues[dialogueIndex]
                .c_str(),
                130,
                GetScreenHeight() - 140,
                24,
                WHITE
            );
        }
    }
}
// ============================================================
// DRAW WORLD
// ============================================================
//
// Story now draws ONLY guards.
//
// Doors are no longer drawn here.
// Tile 20 / 21 and your new tile doors remain handled
// by the tile/renderer system.
// ============================================================

void Story::DrawWorld(
    const Player& player,
    const float zBuffer[],
    int screenWidth,
    int screenHeight,
    bool insideNeon
) const
{
    if (!insideNeon)
        return;

    // ========================================================
    // GUARDS
    // ========================================================

    for (int i = 0; i < 3; i++)
    {
        DrawGuard(
            guards[i],
            player,
            zBuffer,
            screenWidth,
            screenHeight
        );
    }

}

// ============================================================
// DRAW GUARD
// ============================================================

void Story::DrawGuard(
    const Guard& guard,
    const Player& player,
    const float zBuffer[],
    int screenWidth,
    int screenHeight
) const
{
    if (guard.texture.id == 0)
        return;

    if (guardFrameWidth <= 0.0f ||
        guardFrameHeight <= 0.0f)
    {
        return;
    }

    Vector2 playerPos =
        player.GetPosition();

    Vector2 playerDir =
        player.GetDirection();

    Vector2 cameraPlane =
        player.GetCameraPlane();

    Vector2 sprite =
    {
        guard.position.x - playerPos.x,
        guard.position.y - playerPos.y
    };

    // ========================================================
    // CAMERA TRANSFORMATION
    // ========================================================

    float determinant =
        cameraPlane.x * playerDir.y -
        cameraPlane.y * playerDir.x;

    if (fabsf(determinant) < 0.0001f)
        return;

    float invDet =
        1.0f / determinant;

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

    if (transformY <= 0.1f)
        return;

    // ========================================================
    // SCREEN POSITION
    // ========================================================

    int spriteScreenX =
        (int)
        (
            (screenWidth / 2.0f) *
            (
                1.0f +
                transformX / transformY
                )
            );

    // ========================================================
// SPRITE SIZE
//
// Same physical rendering scale as normal NPCs.
//
// NPC uses:
//     spriteScale = 0.55f
// ========================================================

    float spriteScale = 0.95f;

    int spriteHeight =
        abs(
            (int)
            (
                (screenHeight / transformY) *
                spriteScale
                )
        );

    if (spriteHeight <= 0)
        return;

    // --------------------------------------------------------
    // Keep original sprite proportions
    // --------------------------------------------------------

    float aspectRatio =
        guardFrameWidth /
        guardFrameHeight;

    int spriteWidth =
        (int)
        (
            spriteHeight *
            aspectRatio
            );

    // ========================================================
    // VERTICAL POSITION — KEEP GUARD ON THE FLOOR
    // ========================================================

    float groundOffset =
        spriteHeight * 0.5f;

    int drawEndY =
        (int)(screenHeight / 2.0f + groundOffset);

    int drawStartY =
        drawEndY - spriteHeight;

    // --------------------------------------------------------
    // Clamp vertically
    // --------------------------------------------------------

    if (drawStartY < 0)
        drawStartY = 0;

    if (drawEndY >= screenHeight)
        drawEndY = screenHeight - 1;

    if (drawStartY >= screenHeight ||
        drawEndY < 0)
    {
        return;
    }
    // ========================================================
    // HORIZONTAL POSITION
    // ========================================================

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

    if (drawStartX >= screenWidth ||
        drawEndX < 0)
    {
        return;
    }

    // ========================================================
    // FRAME
    //
    // Sprite sheet:
    //
    // [FRAME 1][FRAME 2][FRAME 3][FRAME 4][FRAME 5]
    //
    // Internal indexes:
    //
    // 0 = Frame 1
    // 1 = Frame 2
    // 2 = Frame 3
    // 3 = Frame 4
    // 4 = Frame 5
    // ========================================================

    int frame =
        guard.frame;

    if (frame < 0)
        frame = 0;

    if (frame > 4)
        frame = 4;

    float frameWidth =
        (float)guard.texture.width /
        5.0f;

    float frameHeight =
        (float)guard.texture.height;

    float frameOffsetX =
        frame * frameWidth;

    // ========================================================
    // DRAW VERTICAL STRIPES
    // ========================================================

    for (
        int stripe = drawStartX;
        stripe < drawEndX;
        stripe++
        )
    {
        if (stripe < 0 ||
            stripe >= screenWidth)
        {
            continue;
        }

        // Behind wall/object.
        if (transformY >= zBuffer[stripe])
            continue;

        int trueStartX =
            -spriteWidth / 2 +
            spriteScreenX;

        int texX =
            (int)
            (
                (stripe - trueStartX) *
                frameWidth /
                spriteWidth
                );

        if (texX < 0)
            texX = 0;

        if (texX >= (int)frameWidth)
            texX =
            (int)frameWidth - 1;

        Rectangle source =
        {
            frameOffsetX + texX,
            0.0f,
            1.0f,
            frameHeight
        };

        Rectangle destination =
        {
            (float)stripe,
            (float)drawStartY,
            1.0f,
            (float)(drawEndY - drawStartY)
        };

        DrawTexturePro(
            guard.texture,
            source,
            destination,
            { 0.0f, 0.0f },
            0.0f,
            WHITE
        );
    }

}
// ============================================================
// PLAYER ↔ GUARD COLLISION
//
// Uses the same physical logic as NPC collision.
//
// Guard radius = 0.25f
// Player radius = player.radius
//
// The player is pushed away from the guard.
// The guard is also pushed slightly in the opposite direction.
// Both movements respect the map collision system.
// ============================================================

void Story::CheckPlayerGuardCollision(
    Player& player,
    Map& map
)
{
    for (int i = 0; i < 3; i++)
    {
        Guard& guard = guards[i];

        float dx =
            player.position.x - guard.position.x;

        float dy =
            player.position.y - guard.position.y;

        float distance =
            sqrtf(
                dx * dx +
                dy * dy
            );

        float radiusSum =
            player.radius + guard.radius;

        if (distance <= 0.0001f ||
            distance >= radiusSum)
        {
            continue;
        }

        float overlap =
            radiusSum - distance;

        float normalX =
            dx / distance;

        float normalY =
            dy / distance;

        // ONLY push the PLAYER.
        // Guard never moves.
        Vector2 playerBumpVelocity =
        {
            normalX * overlap,
            normalY * overlap
        };

        player.position =
            CheckMapCollosion(
                player.position,
                player.radius,
                playerBumpVelocity,
                map
            );
    }
}