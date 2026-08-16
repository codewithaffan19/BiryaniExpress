#pragma once

#include "raylib.h"
#include <string>

class Cutscene
{
public:

    Cutscene();

    // Load the 5 cutscene images, background music, typing sound,
    // and 5 per-scene dialogue voice-over sounds.
    bool Initialize(
        const char* image1Path,
        const char* image2Path,
        const char* image3Path,
        const char* image4Path,
        const char* image5Path,

        const char* bgMusicPath,
        const char* typingSoundPath,

        const char* dialogue1Path,
        const char* dialogue2Path,
        const char* dialogue3Path,
        const char* dialogue4Path,
        const char* dialogue5Path
    );

    // Start / restart cutscene
    void Start(bool neon = false);

    // Update cutscene
    void Update();

    // Draw cutscene
    void Draw();

    // Returns true while cutscene is running
    bool IsPlaying() const;

    // Returns true when cutscene has finished
    bool IsFinished() const;

    // Free textures and sounds
    void Unload();

    bool playing = false;
    bool finished = false;
    bool transitioning = false;

    int currentScene = 0;

    // ============================================================
    // TRANSITION
    // ============================================================

    float transitionTimer = 0.0f;

    float transitionDuration = 1.0f;
    int printedWords = 0;

    float wordTimer = 0.0f;

    float wordDelay = 0.08f;


private:

    // ============================================================
    // IMAGES
    // ============================================================

    Texture2D images[5]{};
    bool imagesLoaded[5]{};

    // ============================================================
    // AUDIO
    //
    // bgMusic loops continuously for the whole cutscene (doesn't
    // restart per scene). typingSound fires once per word as the
    // dialogue types out. dialogueSounds[i] is the voice-over line
    // for scene i, played once when that scene starts.
    // ============================================================

    Music bgMusic{};
    bool bgMusicLoaded = false;

    Sound typingSound{};
    bool typingSoundLoaded = false;

    Sound dialogueSounds[5]{};
    bool dialogueSoundsLoaded[5]{};

    // ============================================================
    // STATE
    // ============================================================

    // ============================================================
    // DIALOGUE
    // ============================================================

    std::string dialogue[5];

    std::string neonDialogue[5];

    bool useNeonDialogue = false;


    // ============================================================
    // HELPERS
    // ============================================================

    void StartScene(int scene);

    // Stops any currently-playing dialogue voice-over and starts
    // the one for this scene.
    void PlayDialogueSound(int scene);

    void UpdateDialogue();

    void DrawSceneImage();

    void DrawDialogueBox();

    void DrawTransition();

    void DrawFadeTransition();

    void DrawSmoothTransition();

    void DrawCameraTransition();

    void SplitDialogueWords(
        const std::string& text,
        int& wordCount
    ) const;
};