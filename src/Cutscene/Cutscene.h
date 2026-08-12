#pragma once

#include "raylib.h"
#include <string>

class Cutscene
{
public:

    Cutscene();

    // Load the 4 cutscene images and their audio effects
    bool Initialize(
        const char* image1Path,
        const char* image2Path,
        const char* image3Path,
        const char* image4Path,

        const char* audio1Path,
        const char* audio2Path,
        const char* audio3Path,
        const char* audio4Path
    );

    // Start / restart cutscene
    void Start();

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

private:

    // ============================================================
    // IMAGES
    // ============================================================

    Texture2D images[4]{};
    bool imagesLoaded[4]{};

    // ============================================================
    // AUDIO
    // ============================================================

    Sound sounds[4]{};
    bool soundsLoaded[4]{};

    // ============================================================
    // STATE
    // ============================================================

    bool playing = false;
    bool finished = false;

    int currentScene = 0;

    // ============================================================
    // TRANSITION
    // ============================================================

    float transitionTimer = 0.0f;

    float transitionDuration = 1.0f;

    bool transitioning = false;

    // ============================================================
    // DIALOGUE
    // ============================================================

    std::string dialogue[4];

    int printedWords = 0;

    float wordTimer = 0.0f;

    float wordDelay = 0.08f;

    // ============================================================
    // HELPERS
    // ============================================================

    void StartScene(int scene);

    void StopCurrentAudio();

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