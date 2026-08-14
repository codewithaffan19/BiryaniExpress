#pragma once

#include "raylib.h"

class Menu
{
public:

    enum class Action
    {
        None,
        Play,
        Resume,
        Exit
    };

    Menu();

    // Load menu background
    bool Initialize(const char* backgroundPath);

    // Update menu
    Action Update();

    // Draw menu
    void Draw();

    // Free textures
    void Unload();

    // Settings values
    float GetBrightness() const;
    float GetMusicVolume() const;

    bool IsSettingsOpen() const;
    // =====================================================
// PAUSE MENU
// =====================================================

    void OpenPause();
    void ClosePause();

    bool IsPauseOpen() const;

    Action UpdatePause();
    void DrawPause();
    bool backgroundLoaded = false;


    // =====================================================
    // MENU STATE
    // =====================================================

    bool settingsOpen = false;
    bool pauseOpen = false;


    // =====================================================
    // SETTINGS
    // =====================================================

    float brightness = 1.0f;
    float musicVolume = 1.0f;

private:

    // =====================================================
    // BACKGROUND
    // =====================================================

    Texture2D backgroundTexture{};
    

    // =====================================================
    // ANIMATION
    // =====================================================

    float animationTimer = 0.0f;


    // =====================================================
    // BUTTONS
    // =====================================================

    Rectangle playButton{};
    Rectangle settingsButton{};
    Rectangle exitButton{};

    Rectangle brightnessMinusButton{};
    Rectangle brightnessPlusButton{};

    Rectangle musicMinusButton{};
    Rectangle musicPlusButton{};

    Rectangle backButton{};
    Rectangle resumeButton{};
    Rectangle pauseExitButton{};

    // =====================================================
    // HELPERS
    // =====================================================

    void UpdateButtonPositions();

    void DrawMainMenu();
    void DrawPauseMenu();
    void DrawSettings();

    void DrawLogo();

    void DrawMarqueeLights();

    void DrawButton(
        Rectangle rect,
        const char* text,
        bool hovered,
        bool selected = false
    );

    bool IsMouseOver(Rectangle rect) const;

    Rectangle GetHoveredButton() const;
};