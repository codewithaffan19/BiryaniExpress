#pragma once

#include "raylib.h"
#include "../Player/Player.h"
#include<string>
class Story
{
public:

    Story();

    void Initialize();
    void Unload();

    // ============================================================
    // STORY LOGIC
    // ============================================================

    void Update(
        Player& player,
        bool insideNeon
    );

    bool IsPlayerLocked() const;
    bool IsWon() const;

    // ============================================================
    // KEY / TILE-DOOR LOGIC
    //
    // Tile doors are rendered and handled by the tile system.
    // Story only owns the keys.
    // ============================================================

    bool HasKeyForDoor(int doorNumber) const;

    // ============================================================
    // UI
    // ============================================================

    void DrawUI() const;

    // ============================================================
    // WORLD RENDERING
    //
    // Story only renders guards.
    // Doors are now tile doors.
    // ============================================================

    void DrawWorld(
        const Player& player,
        const float zBuffer[],
        int screenWidth,
        int screenHeight,
        bool insideNeon
    ) const;
    // ============================================================
// DOOR SYSTEM
// ============================================================

    bool IsDoorOpened(int doorNumber) const;

    bool TryUseDoor(int doorNumber);

    bool HasAnyKey() const;

    Vector2 GetTeleportPosition(int doorNumber) const;

    bool WasPoliceSpawnTriggered() const;

private:

    // ============================================================
    // STORY KEY
    // ============================================================

    enum StoryKey
    {
        KEY_NONE = 0,
        KEY_DOOR_1,
        KEY_DOOR_2,
        KEY_DOOR_3
    };

    // ============================================================
    // GUARD
    // ============================================================

    struct Guard
    {
        Vector2 position;

        StoryKey key;

        // --------------------------------------------------------
        // Interaction state
        // --------------------------------------------------------

        // 0 = waiting for first E
        // 1 = waiting for second E
        // 2 = animation frames 2,3,4 looping
        // 3 = frame 5 / key collected
        // 4 = permanently completed
        int interactionStage = 0;

        bool interacting = false;

        float interactionTimer = 0.0f;

        // --------------------------------------------------------
        // Sprite frame
        //
        // 0 = Frame 1 / idle
        // 1 = Frame 2
        // 2 = Frame 3
        // 3 = Frame 4
        // 4 = Frame 5
        // --------------------------------------------------------

        int frame = 0;

        // --------------------------------------------------------
        // Prevent another interaction after completion.
        // --------------------------------------------------------

        bool completed = false;

        // --------------------------------------------------------
        // Each guard has its own sprite sheet.
        // --------------------------------------------------------

        Texture2D texture{};
    };

    // ============================================================
    // THREE GUARDS
    // ============================================================

    Guard guards[3];

    // ============================================================
    // GUARD SPRITE INFORMATION
    // ============================================================

    float guardFrameWidth = 0.0f;
    float guardFrameHeight = 0.0f;

    // ============================================================
    // GUARD SIZE
    //
    // Increase this value to make guards bigger.
    //
    // Example:
    // 0.55f = current-ish size
    // 0.70f = bigger
    // 0.85f = much bigger
    // 1.00f = very big
    // ============================================================

    static constexpr float GUARD_SCALE = 2.05f;

    // ============================================================
    // GUARD FLOOR OFFSET
    //
    // Positive value moves the guard DOWN.
    // Negative value moves the guard UP.
    //
    // Start with 0.0f.
    // ============================================================

    static constexpr float GUARD_FLOOR_OFFSET = 250.0f;

    // ============================================================
    // CURRENT KEY
    // ============================================================

    StoryKey currentKey = KEY_NONE;

    // ============================================================
    // STORY STATE
    // ============================================================

    bool playerLocked = false;

    bool won = false;

    bool dialogueBox = false;

    int activeGuard = -1;

    float interactionDistance = 2.0f;
    // ============================================================
// ON-SCREEN STORY MESSAGE
// ============================================================

    std::string screenMessage = "";

    float screenMessageTimer = 0.0f;

    int currentDoor = -1;
    // ============================================================
    // POSITIONS
    // ============================================================

    Vector2 winPosition{};

    // ============================================================
    // TEXTURES
    // ============================================================

    void LoadStoryTextures();

    // ============================================================
    // GUARD LOGIC
    // ============================================================

    void UpdateGuardInteraction(
        Player& player,
        int guardIndex
    );

    void StartGuardInteraction(
        int guardIndex
    );

    void GiveKey(
        int guardIndex
    );

    // ============================================================
    // HELPERS
    // ============================================================

    bool IsNear(
        Vector2 a,
        Vector2 b,
        float distance
    ) const;

    void PrintKeyName(
        StoryKey key
    ) const;

    void SetScreenMessage(
        const std::string& message,
        float duration = 2.0f
    );

    // ============================================================
    // WORLD DRAWING
    // ============================================================

    void DrawGuard(
        const Guard& guard,
        const Player& player,
        const float zBuffer[],
        int screenWidth,
        int screenHeight
    ) const;
    // ============================================================
// STORY DOORS
//
// 0 = Door 1
// 1 = Door 2
// 2 = Door 3
// ============================================================

    bool doorOpened[3] =
    {
        false,
        false,
        false
    };

    // ============================================================
    // POLICE EVENT
    // ============================================================

    bool policeSpawnTriggered = false;

    // ============================================================
    // DOOR POSITIONS
    // ============================================================

    Vector2 doorPositions[3];

    // ============================================================
    // TELEPORT DESTINATIONS
    // ============================================================

    Vector2 doorTeleportPositions[3];
};