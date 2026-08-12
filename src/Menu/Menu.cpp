#include "Menu.h"

#include <cmath>

Menu::Menu()
{
    brightness = 1.0f;
    musicVolume = 1.0f;

    settingsOpen = false;
    pauseOpen = false;

    animationTimer = 0.0f;
}


// ============================================================
// INITIALIZE
// ============================================================

bool Menu::Initialize(const char* backgroundPath)
{
    if (backgroundPath == nullptr)
        return false;

    backgroundTexture = LoadTexture(backgroundPath);

    if (backgroundTexture.id == 0)
    {
        TraceLog(
            LOG_ERROR,
            "Failed to load menu background: %s",
            backgroundPath
        );

        backgroundLoaded = false;
        return false;
    }

    SetTextureFilter(
        backgroundTexture,
        TEXTURE_FILTER_BILINEAR
    );

    backgroundLoaded = true;

    UpdateButtonPositions();

    return true;
}


// ============================================================
// BUTTON POSITIONS
// ============================================================

void Menu::UpdateButtonPositions()
{
    float screenWidth = (float)GetScreenWidth();
    float screenHeight = (float)GetScreenHeight();

    // Main menu buttons
    float buttonWidth = 480.0f;
    float buttonHeight = 82.0f;

    float buttonX =
        screenWidth / 2.0f - buttonWidth / 2.0f;

    float startY =
        screenHeight * 0.58f;

    playButton =
    {
        buttonX,
        startY,
        buttonWidth,
        buttonHeight
    };

    settingsButton =
    {
        buttonX,
        startY + 105.0f,
        buttonWidth,
        buttonHeight
    };

    exitButton =
    {
        buttonX,
        startY + 210.0f,
        buttonWidth,
        buttonHeight
    };


    // Settings buttons

    float settingsButtonWidth = 70.0f;
    float settingsButtonHeight = 55.0f;

    float centerX = screenWidth / 2.0f;

    brightnessMinusButton =
    {
        centerX - 210.0f,
        screenHeight * 0.46f,
        settingsButtonWidth,
        settingsButtonHeight
    };

    brightnessPlusButton =
    {
        centerX + 140.0f,
        screenHeight * 0.46f,
        settingsButtonWidth,
        settingsButtonHeight
    };


    musicMinusButton =
    {
        centerX - 210.0f,
        screenHeight * 0.56f,
        settingsButtonWidth,
        settingsButtonHeight
    };

    musicPlusButton =
    {
        centerX + 140.0f,
        screenHeight * 0.56f,
        settingsButtonWidth,
        settingsButtonHeight
    };


    backButton =
    {
        centerX - 240.0f,
        screenHeight * 0.72f,
        480.0f,
        75.0f
    };
    // ========================================================
// PAUSE MENU BUTTONS
// ========================================================

    float pauseButtonWidth = 480.0f;
    float pauseButtonHeight = 82.0f;

    float pauseButtonX =
        screenWidth / 2.0f - pauseButtonWidth / 2.0f;

    float pauseStartY =
        screenHeight * 0.48f;

    resumeButton =
    {
        pauseButtonX,
        pauseStartY,
        pauseButtonWidth,
        pauseButtonHeight
    };

    pauseExitButton =
    {
        pauseButtonX,
        pauseStartY + 105.0f,
        pauseButtonWidth,
        pauseButtonHeight
    };
}


// ============================================================
// MOUSE CHECK
// ============================================================

bool Menu::IsMouseOver(Rectangle rect) const
{
    Vector2 mouse = GetMousePosition();

    return CheckCollisionPointRec(
        mouse,
        rect
    );
}


// ============================================================
// UPDATE
// ============================================================

Menu::Action Menu::Update()
{
    animationTimer += GetFrameTime();

    UpdateButtonPositions();


    // ========================================================
    // SETTINGS SCREEN
    // ========================================================

    if (settingsOpen)
    {
        // Brightness -
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            IsMouseOver(brightnessMinusButton))
        {
            brightness -= 0.1f;

            if (brightness < 0.2f)
                brightness = 0.2f;
        }


        // Brightness +
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            IsMouseOver(brightnessPlusButton))
        {
            brightness += 0.1f;

            if (brightness > 1.0f)
                brightness = 1.0f;
        }


        // Music -
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            IsMouseOver(musicMinusButton))
        {
            musicVolume -= 0.1f;

            if (musicVolume < 0.0f)
                musicVolume = 0.0f;
        }


        // Music +
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            IsMouseOver(musicPlusButton))
        {
            musicVolume += 0.1f;

            if (musicVolume > 1.0f)
                musicVolume = 1.0f;
        }


        // Back
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            IsMouseOver(backButton))
        {
            settingsOpen = false;
        }


        // ESC also returns to main menu
        if (IsKeyPressed(KEY_ESCAPE))
        {
            settingsOpen = false;
        }

        return Action::None;
    }


    // ========================================================
    // MAIN MENU
    // ========================================================

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if (IsMouseOver(playButton))
        {
            return Action::Play;
        }

        if (IsMouseOver(settingsButton))
        {
            settingsOpen = true;
            return Action::None;
        }

        if (IsMouseOver(exitButton))
        {
            return Action::Exit;
        }
    }

    return Action::None;
}


// ============================================================
// DRAW
// ============================================================

void Menu::Draw()
{
    UpdateButtonPositions();

    // --------------------------------------------------------
    // BACKGROUND
    // --------------------------------------------------------

    if (backgroundLoaded)
    {
        Rectangle source =
        {
            0,
            0,
            (float)backgroundTexture.width,
            (float)backgroundTexture.height
        };

        Rectangle destination =
        {
            0,
            0,
            (float)GetScreenWidth(),
            (float)GetScreenHeight()
        };

        DrawTexturePro(
            backgroundTexture,
            source,
            destination,
            { 0, 0 },
            0.0f,
            WHITE
        );
    }
    else
    {
        ClearBackground(
            Color{ 5, 10, 20, 255 }
        );
    }


    // --------------------------------------------------------
    // SETTINGS / MAIN MENU
    // --------------------------------------------------------

    if (settingsOpen)
        DrawSettings();
    else
        DrawMainMenu();


    // --------------------------------------------------------
    // BRIGHTNESS OVERLAY
    // --------------------------------------------------------

    float darkness =
        1.0f - brightness;

    if (darkness > 0.01f)
    {
        DrawRectangle(
            0,
            0,
            GetScreenWidth(),
            GetScreenHeight(),
            Fade(BLACK, darkness * 0.75f)
        );
    }
}


// ============================================================
// MAIN MENU
// ============================================================

void Menu::DrawMainMenu()
{
    DrawLogo();


    // --------------------------------------------------------
    // BUTTONS
    // --------------------------------------------------------

    bool playHovered =
        IsMouseOver(playButton);

    bool settingsHovered =
        IsMouseOver(settingsButton);

    bool exitHovered =
        IsMouseOver(exitButton);


    DrawButton(
        playButton,
        "PLAY",
        playHovered
    );

    DrawButton(
        settingsButton,
        "SETTINGS",
        settingsHovered
    );

    DrawButton(
        exitButton,
        "EXIT",
        exitHovered
    );


    // --------------------------------------------------------
    // SIMPLE ICONS
    // --------------------------------------------------------

    // Play icon
    Vector2 playCenter =
    {
        playButton.x + 95.0f,
        playButton.y + playButton.height / 2.0f
    };

    DrawTriangle(
        {
            playCenter.x - 15,
            playCenter.y - 23
        },
        {
            playCenter.x - 15,
            playCenter.y + 23
        },
        {
            playCenter.x + 22,
            playCenter.y
        },
        GOLD
    );


    // Settings gear - simple Raylib-friendly representation
    Vector2 gearCenter =
    {
        settingsButton.x + 95.0f,
        settingsButton.y + settingsButton.height / 2.0f
    };

    DrawCircle(
        (int)gearCenter.x,
        (int)gearCenter.y,
        20,
        GOLD
    );

    DrawCircle(
        (int)gearCenter.x,
        (int)gearCenter.y,
        9,
        Color{ 8, 15, 25, 255 }
    );


    // Exit icon
    Vector2 exitCenter =
    {
        exitButton.x + 95.0f,
        exitButton.y + exitButton.height / 2.0f
    };

    DrawRectangleLinesEx(
        {
            exitCenter.x - 20,
            exitCenter.y - 22,
            32,
            44
        },
        4,
        GOLD
    );

    DrawLineEx(
        {
            exitCenter.x - 5,
            exitCenter.y
        },
        {
            exitCenter.x + 28,
            exitCenter.y
        },
        4,
        GOLD
    );

    DrawTriangle(
        {
            exitCenter.x + 30,
            exitCenter.y
        },
        {
            exitCenter.x + 15,
            exitCenter.y - 10
        },
        {
            exitCenter.x + 15,
            exitCenter.y + 10
        },
        GOLD
    );
}


// ============================================================
// LOGO
// ============================================================

void Menu::DrawLogo()
{
    float screenWidth =
        (float)GetScreenWidth();

    float screenHeight =
        (float)GetScreenHeight();


    float logoWidth = 820.0f;
    float logoHeight = 260.0f;

    Rectangle logo =
    {
        screenWidth / 2.0f - logoWidth / 2.0f,
        screenHeight * 0.08f,
        logoWidth,
        logoHeight
    };


    // --------------------------------------------------------
    // Outer dark marquee
    // --------------------------------------------------------

    DrawRectangleRounded(
        logo,
        0.08f,
        20,
        Color{ 5, 10, 18, 235 }
    );


    // Gold border
    DrawRectangleRoundedLinesEx(
        logo,
        0.08f,
        20,
        5.0f,
        GOLD
    );


    Rectangle inner =
    {
        logo.x + 15,
        logo.y + 15,
        logo.width - 30,
        logo.height - 30
    };

    DrawRectangleRoundedLinesEx(
        inner,
        0.07f,
        20,
        2.0f,
        Color{ 170, 115, 25, 255 }
    );


    // --------------------------------------------------------
    // Blinking bulbs
    // --------------------------------------------------------

    DrawMarqueeLights();


    // --------------------------------------------------------
    // LAST KEY
    // --------------------------------------------------------

    const char* title =
        "LAST KEY";

    int titleFontSize = 76;

    int titleWidth =
        MeasureText(
            title,
            titleFontSize
        );

    DrawText(
        title,
        (int)(screenWidth / 2.0f - titleWidth / 2.0f),
        (int)(logo.y + 50),
        titleFontSize,
        WHITE
    );


    // --------------------------------------------------------
    // Subtitle
    // --------------------------------------------------------

    const char* subtitle =
        "THE KARACHI STREET SURVIVAL";

    int subtitleFontSize = 26;

    int subtitleWidth =
        MeasureText(
            subtitle,
            subtitleFontSize
        );

    DrawText(
        subtitle,
        (int)(screenWidth / 2.0f - subtitleWidth / 2.0f),
        (int)(logo.y + 175),
        subtitleFontSize,
        WHITE
    );


    // Small gold lines beside subtitle

    DrawRectangle(
        (int)(screenWidth / 2.0f - subtitleWidth / 2.0f - 95),
        (int)(logo.y + 186),
        65,
        3,
        GOLD
    );

    DrawRectangle(
        (int)(screenWidth / 2.0f + subtitleWidth / 2.0f + 30),
        (int)(logo.y + 186),
        65,
        3,
        GOLD
    );
}


// ============================================================
// MARQUEE LIGHTS
// ============================================================

void Menu::DrawMarqueeLights()
{
    float screenWidth =
        (float)GetScreenWidth();

    float screenHeight =
        (float)GetScreenHeight();


    float logoWidth = 820.0f;
    float logoHeight = 260.0f;

    Rectangle logo =
    {
        screenWidth / 2.0f - logoWidth / 2.0f,
        screenHeight * 0.08f,
        logoWidth,
        logoHeight
    };


    const int bulbsPerSide = 12;


    // --------------------------------------------------------
    // Top + Bottom
    // --------------------------------------------------------

    for (int i = 0; i < bulbsPerSide; i++)
    {
        float t =
            (float)i / (float)(bulbsPerSide - 1);

        float x =
            logo.x + 35.0f +
            t * (logo.width - 70.0f);


        // Different phase for each bulb
        float phase =
            animationTimer * 5.0f +
            i * 0.45f;

        float glow =
            0.55f +
            0.45f * sinf(phase);


        Color bulbColor =
        {
            255,
            190,
            45,
            (unsigned char)(170 + glow * 85)
        };


        // Top
        DrawCircle(
            (int)x,
            (int)(logo.y + 18),
            6.0f,
            bulbColor
        );


        // Bottom
        DrawCircle(
            (int)x,
            (int)(logo.y + logo.height - 18),
            6.0f,
            bulbColor
        );
    }


    // --------------------------------------------------------
    // Left + Right
    // --------------------------------------------------------

    for (int i = 0; i < bulbsPerSide; i++)
    {
        float t =
            (float)i / (float)(bulbsPerSide - 1);

        float y =
            logo.y + 35.0f +
            t * (logo.height - 70.0f);


        float phase =
            animationTimer * 5.0f +
            i * 0.45f +
            2.0f;


        float glow =
            0.55f +
            0.45f * sinf(phase);


        Color bulbColor =
        {
            255,
            190,
            45,
            (unsigned char)(170 + glow * 85)
        };


        DrawCircle(
            (int)(logo.x + 18),
            (int)y,
            6.0f,
            bulbColor
        );


        DrawCircle(
            (int)(logo.x + logo.width - 18),
            (int)y,
            6.0f,
            bulbColor
        );
    }
}


// ============================================================
// BUTTON DRAWING
// ============================================================

void Menu::DrawButton(
    Rectangle rect,
    const char* text,
    bool hovered,
    bool selected)
{
    float scale =
        hovered ? 1.045f : 1.0f;


    float newWidth =
        rect.width * scale;

    float newHeight =
        rect.height * scale;


    Rectangle drawRect =
    {
        rect.x - (newWidth - rect.width) / 2.0f,
        rect.y - (newHeight - rect.height) / 2.0f,
        newWidth,
        newHeight
    };


    // --------------------------------------------------------
    // Button background
    // --------------------------------------------------------

    Color background =
        hovered
        ? Color{ 12, 25, 40, 245 }
    : Color{ 5, 15, 25, 235 };


    DrawRectangleRounded(
        drawRect,
        0.12f,
        15,
        background
    );


    // --------------------------------------------------------
    // Golden border
    // --------------------------------------------------------

    DrawRectangleRoundedLinesEx(
        drawRect,
        0.12f,
        15,
        hovered ? 4.0f : 3.0f,
        GOLD
    );


    // --------------------------------------------------------
    // Hover glow
    // --------------------------------------------------------

    if (hovered)
    {
        DrawRectangleRoundedLinesEx(
            {
                drawRect.x - 4,
                drawRect.y - 4,
                drawRect.width + 8,
                drawRect.height + 8
            },
            0.12f,
            15,
            2.0f,
            Fade(GOLD, 0.35f)
        );
    }


    // --------------------------------------------------------
    // Text
    // --------------------------------------------------------

    int fontSize = 34;

    int textWidth =
        MeasureText(
            text,
            fontSize
        );


    DrawText(
        text,
        (int)(
            drawRect.x +
            drawRect.width / 2.0f -
            textWidth / 2.0f +
            25.0f
            ),
        (int)(
            drawRect.y +
            drawRect.height / 2.0f -
            fontSize / 2.0f
            ),
        fontSize,
        WHITE
    );
}


// ============================================================
// SETTINGS
// ============================================================

void Menu::DrawSettings()
{
    float screenWidth =
        (float)GetScreenWidth();

    float screenHeight =
        (float)GetScreenHeight();


    // --------------------------------------------------------
    // Title
    // --------------------------------------------------------

    const char* title =
        "SETTINGS";

    int titleSize = 64;

    int titleWidth =
        MeasureText(
            title,
            titleSize
        );

    DrawText(
        title,
        (int)(screenWidth / 2.0f - titleWidth / 2.0f),
        (int)(screenHeight * 0.15f),
        titleSize,
        WHITE
    );


    // --------------------------------------------------------
    // Brightness
    // --------------------------------------------------------

    const char* brightnessText =
        "BRIGHTNESS";

    DrawText(
        brightnessText,
        (int)(screenWidth / 2.0f - 150),
        (int)(screenHeight * 0.40f),
        28,
        WHITE
    );


    // Value
    char brightnessValue[32];

    TextFormat(
        "%d%%",
        (int)(brightness * 100.0f)
    );

    DrawText(
        TextFormat(
            "%d%%",
            (int)(brightness * 100.0f)
        ),
        (int)(screenWidth / 2.0f - 20),
        (int)(screenHeight * 0.47f),
        28,
        WHITE
    );


    // --------------------------------------------------------
    // Music
    // --------------------------------------------------------

    DrawText(
        "MUSIC VOLUME",
        (int)(screenWidth / 2.0f - 150),
        (int)(screenHeight * 0.50f),
        28,
        WHITE
    );


    DrawText(
        TextFormat(
            "%d%%",
            (int)(musicVolume * 100.0f)
        ),
        (int)(screenWidth / 2.0f - 20),
        (int)(screenHeight * 0.57f),
        28,
        WHITE
    );


    // --------------------------------------------------------
    // Minus / Plus buttons
    // --------------------------------------------------------

    DrawButton(
        brightnessMinusButton,
        "-",
        IsMouseOver(brightnessMinusButton)
    );

    DrawButton(
        brightnessPlusButton,
        "+",
        IsMouseOver(brightnessPlusButton)
    );

    DrawButton(
        musicMinusButton,
        "-",
        IsMouseOver(musicMinusButton)
    );

    DrawButton(
        musicPlusButton,
        "+",
        IsMouseOver(musicPlusButton)
    );


    // --------------------------------------------------------
    // Back
    // --------------------------------------------------------

    DrawButton(
        backButton,
        "BACK",
        IsMouseOver(backButton)
    );
}


// ============================================================
// GETTERS
// ============================================================

float Menu::GetBrightness() const
{
    return brightness;
}


float Menu::GetMusicVolume() const
{
    return musicVolume;
}


bool Menu::IsSettingsOpen() const
{
    return settingsOpen;
}

// ============================================================
// PAUSE MENU
// ============================================================

void Menu::OpenPause()
{
    pauseOpen = true;
    settingsOpen = false;

    EnableCursor();
}
void Menu::ClosePause()
{
    pauseOpen = false;

    DisableCursor();
}
bool Menu::IsPauseOpen() const
{
    return pauseOpen;
}
Menu::Action Menu::UpdatePause()
{
    UpdateButtonPositions();

    // ========================================================
    // RESUME
    // ========================================================

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
        IsMouseOver(resumeButton))
    {
        ClosePause();

        return Action::Resume;
    }


    // ========================================================
    // EXIT
    // ========================================================

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
        IsMouseOver(pauseExitButton))
    {
        return Action::Exit;
    }


    // ========================================================
    // ESC = RESUME
    // ========================================================

    if (IsKeyPressed(KEY_ESCAPE))
    {
        ClosePause();

        return Action::Resume;
    }


    return Action::None;
}
void Menu::DrawPause()
{
    UpdateButtonPositions();

    // ========================================================
    // SAME BACKGROUND AS MAIN MENU
    // ========================================================

    if (backgroundLoaded)
    {
        Rectangle source =
        {
            0,
            0,
            (float)backgroundTexture.width,
            (float)backgroundTexture.height
        };

        Rectangle destination =
        {
            0,
            0,
            (float)GetScreenWidth(),
            (float)GetScreenHeight()
        };

        DrawTexturePro(
            backgroundTexture,
            source,
            destination,
            { 0, 0 },
            0.0f,
            WHITE
        );
    }
    else
    {
        ClearBackground(
            Color{ 5, 10, 20, 255 }
        );
    }


    // ========================================================
    // PAUSE MENU
    // ========================================================

    DrawPauseMenu();


    // ========================================================
    // BRIGHTNESS
    // ========================================================

    float darkness =
        1.0f - brightness;

    if (darkness > 0.01f)
    {
        DrawRectangle(
            0,
            0,
            GetScreenWidth(),
            GetScreenHeight(),
            Fade(BLACK, darkness * 0.75f)
        );
    }
}
void Menu::DrawPauseMenu()
{
    float screenWidth =
        (float)GetScreenWidth();

    float screenHeight =
        (float)GetScreenHeight();


    // ========================================================
    // TITLE
    // ========================================================

    const char* title = "PAUSED";

    int titleSize = 64;

    int titleWidth =
        MeasureText(
            title,
            titleSize
        );

    DrawText(
        title,
        (int)(
            screenWidth / 2.0f -
            titleWidth / 2.0f
            ),
        (int)(screenHeight * 0.20f),
        titleSize,
        WHITE
    );


    // ========================================================
    // RESUME
    // ========================================================

    DrawButton(
        resumeButton,
        "RESUME",
        IsMouseOver(resumeButton)
    );


    // ========================================================
    // EXIT
    // ========================================================

    DrawButton(
        pauseExitButton,
        "EXIT",
        IsMouseOver(pauseExitButton)
    );
}
// ============================================================
// UNLOAD
// ============================================================

void Menu::Unload()
{
    if (backgroundLoaded)
    {
        UnloadTexture(backgroundTexture);

        backgroundTexture = {};
        backgroundLoaded = false;
    }
}
