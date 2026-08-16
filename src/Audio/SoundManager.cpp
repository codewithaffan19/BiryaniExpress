#include "SoundManager.h"

// ============================================================
// CONSTRUCTOR
// ============================================================

SoundManager::SoundManager()
{
}


// ============================================================
// LOAD
// ============================================================

void SoundManager::LoadSounds(
    const char* menuPath,
    const char* streetPath,
    const char* marketPath,
    const char* neonPath
)
{
    if (menuPath != nullptr)
    {
        menuMusic = LoadMusicStream(menuPath);

        if (menuMusic.frameCount == 0)
        {
            TraceLog(
                LOG_WARNING,
                "Failed to load menu ambience: %s",
                menuPath
            );

            menuLoaded = false;
        }
        else
        {
            menuMusic.looping = true;
            menuLoaded = true;
        }
    }

    if (streetPath != nullptr)
    {
        streetMusic = LoadMusicStream(streetPath);

        if (streetMusic.frameCount == 0)
        {
            TraceLog(
                LOG_WARNING,
                "Failed to load street ambience: %s",
                streetPath
            );

            streetLoaded = false;
        }
        else
        {
            streetMusic.looping = true;
            streetLoaded = true;
        }
    }

    if (marketPath != nullptr)
    {
        marketMusic = LoadMusicStream(marketPath);

        if (marketMusic.frameCount == 0)
        {
            TraceLog(
                LOG_WARNING,
                "Failed to load market ambience: %s",
                marketPath
            );

            marketLoaded = false;
        }
        else
        {
            marketMusic.looping = true;
            marketLoaded = true;
        }
    }

    if (neonPath != nullptr)
    {
        neonMusic = LoadMusicStream(neonPath);

        if (neonMusic.frameCount == 0)
        {
            TraceLog(
                LOG_WARNING,
                "Failed to load neon ambience: %s",
                neonPath
            );

            neonLoaded = false;
        }
        else
        {
            neonMusic.looping = true;
            neonLoaded = true;
        }
    }
}


// ============================================================
// HELPERS
// ============================================================

Music* SoundManager::GetMusic(Ambience ambience)
{
    switch (ambience)
    {
    case Ambience::Menu:   return &menuMusic;
    case Ambience::Street: return &streetMusic;
    case Ambience::Market: return &marketMusic;
    case Ambience::Neon:   return &neonMusic;
    default:               return nullptr;
    }
}

bool SoundManager::IsLoaded(Ambience ambience) const
{
    switch (ambience)
    {
    case Ambience::Menu:   return menuLoaded;
    case Ambience::Street: return streetLoaded;
    case Ambience::Market: return marketLoaded;
    case Ambience::Neon:   return neonLoaded;
    default:               return false;
    }
}


// ============================================================
// SET AMBIENCE
// ============================================================

void SoundManager::SetAmbience(Ambience ambience)
{
    if (ambience == currentAmbience)
        return;

    Music* current = GetMusic(currentAmbience);

    if (current != nullptr &&
        IsLoaded(currentAmbience) &&
        IsMusicStreamPlaying(*current))
    {
        StopMusicStream(*current);
    }

    currentAmbience = ambience;

    Music* next = GetMusic(currentAmbience);

    if (next != nullptr && IsLoaded(currentAmbience))
    {
        PlayMusicStream(*next);
    }
}


// ============================================================
// STOP
// ============================================================

void SoundManager::Stop()
{
    Music* current = GetMusic(currentAmbience);

    if (current != nullptr && IsLoaded(currentAmbience))
    {
        StopMusicStream(*current);
    }

    currentAmbience = Ambience::None;
}


// ============================================================
// UPDATE (call once per frame)
// ============================================================

void SoundManager::Update()
{
    Music* current = GetMusic(currentAmbience);

    if (current != nullptr && IsLoaded(currentAmbience))
    {
        UpdateMusicStream(*current);
    }
}


// ============================================================
// VOLUME
// ============================================================

void SoundManager::SetVolume(float volume)
{
    if (menuLoaded)   SetMusicVolume(menuMusic, volume);
    if (streetLoaded) SetMusicVolume(streetMusic, volume);
    if (marketLoaded) SetMusicVolume(marketMusic, volume);
    if (neonLoaded)   SetMusicVolume(neonMusic, volume);
}


// ============================================================
// SFX: FOOTSTEP + HIT
// ============================================================

void SoundManager::LoadSfx(
    const char* footstepPath,
    const char* hitPath
)
{
    if (footstepPath != nullptr)
    {
        footstepSound = LoadSound(footstepPath);

        if (footstepSound.frameCount == 0)
        {
            TraceLog(
                LOG_WARNING,
                "Failed to load footstep sound: %s",
                footstepPath
            );

            footstepLoaded = false;
        }
        else
        {
            footstepLoaded = true;
        }
    }

    if (hitPath != nullptr)
    {
        hitSound = LoadSound(hitPath);

        if (hitSound.frameCount == 0)
        {
            TraceLog(
                LOG_WARNING,
                "Failed to load hit sound: %s",
                hitPath
            );

            hitLoaded = false;
        }
        else
        {
            hitLoaded = true;
        }
    }
}

void SoundManager::PlayFootstep()
{
    if (footstepLoaded)
    {
        PlaySound(footstepSound);
    }
}

void SoundManager::PlayHit()
{
    if (hitLoaded)
    {
        PlaySound(hitSound);
    }
}

void SoundManager::SetSfxVolume(float volume)
{
    if (footstepLoaded) SetSoundVolume(footstepSound, volume);
    if (hitLoaded)      SetSoundVolume(hitSound, volume);
}


// ============================================================
// UNLOAD
// ============================================================

void SoundManager::Unload()
{
    if (menuLoaded)
    {
        UnloadMusicStream(menuMusic);
        menuLoaded = false;
    }

    if (streetLoaded)
    {
        UnloadMusicStream(streetMusic);
        streetLoaded = false;
    }

    if (marketLoaded)
    {
        UnloadMusicStream(marketMusic);
        marketLoaded = false;
    }

    if (neonLoaded)
    {
        UnloadMusicStream(neonMusic);
        neonLoaded = false;
    }

    if (footstepLoaded)
    {
        UnloadSound(footstepSound);
        footstepLoaded = false;
    }

    if (hitLoaded)
    {
        UnloadSound(hitSound);
        hitLoaded = false;
    }

    currentAmbience = Ambience::None;
}
