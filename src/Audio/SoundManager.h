#pragma once

#include "raylib.h"

// ================================================================
// SOUND MANAGER
//
// 4 looping ambience slots: Menu, Street, Market, Neon.
//
// Only one ambience plays at a time. Switching areas automatically
// stops the old ambience and starts the new one (e.g. entering the
// market turns Street off and Market on; leaving turns Market off
// and Street back on).
//
// Missing/failed audio files are handled the same defensive way as
// the rest of the project (Cutscene, NPC, Menu, etc.): a failed
// load is logged once and that slot is silently skipped from then
// on, it never crashes or blocks gameplay.
// ================================================================

class SoundManager
{
public:

    enum class Ambience
    {
        None,
        Menu,
        Street,
        Market,
        Neon
    };

    SoundManager();

    // Load the 4 ambience tracks. Any path can be nullptr / missing;
    // that slot is simply skipped.
    void LoadSounds(
        const char* menuPath,
        const char* streetPath,
        const char* marketPath,
        const char* neonPath
    );

    // Switch to this ambience (no-op if it's already playing).
    void SetAmbience(Ambience ambience);

    // Stop whatever is currently playing.
    void Stop();

    // Call once per frame to keep the current music stream fed.
    void Update();

    // 0.0 - 1.0, applied to all 4 slots (wired to the existing
    // Settings > Music slider).
    void SetVolume(float volume);

    // ============================================================
    // ONE-SHOT SFX: footstep + hit
    //
    // Separate from the looping ambience above — these are short
    // sounds fired on an event (a step, a landed hit), not tracks
    // that play continuously.
    // ============================================================

    // Load the footstep + hit one-shot sounds. Either path can be
    // nullptr / missing; that slot is simply skipped.
    void LoadSfx(
        const char* footstepPath,
        const char* hitPath
    );

    // Plays the footstep sound. Intended to be called on an
    // interval while the player is moving (WASD) — see
    // Game::Update()'s footstepTimer.
    void PlayFootstep();

    // Plays the enemy-hit sound once.
    void PlayHit();

    // 0.0 - 1.0, applied to both SFX slots.
    void SetSfxVolume(float volume);

    // Free all loaded audio.
    void Unload();

private:

    Music menuMusic{};
    Music streetMusic{};
    Music marketMusic{};
    Music neonMusic{};

    bool menuLoaded = false;
    bool streetLoaded = false;
    bool marketLoaded = false;
    bool neonLoaded = false;

    Sound footstepSound{};
    Sound hitSound{};

    bool footstepLoaded = false;
    bool hitLoaded = false;

    Ambience currentAmbience = Ambience::None;

    Music* GetMusic(Ambience ambience);
    bool IsLoaded(Ambience ambience) const;
};
