#include "Cutscene.h"

#include <cmath>
#include <sstream>
#include <vector>

// ============================================================
// CONSTRUCTOR
// ============================================================

Cutscene::Cutscene()
{
    playing = false;
    finished = false;

    currentScene = 0;

    transitionTimer = 0.0f;
    transitioning = false;

    printedWords = 0;
    wordTimer = 0.0f;

    // ========================================================
    // TEMPORARY DIALOGUE
    //
    // Replace these later with your actual dialogue.
    // ========================================================

    dialogue[0] =
        "Affan: 6 se maar isko, phir dobara baari le! Isko aaj bachne nahi dena.";

    dialogue[1] =
        "Rohaan: Chalo shart laga lo, jo haara woh sab ko Drumble aur AFC khilaye ga. Hassan: Done karo bhai!Aaj to jeet ke rahun ga, dekh lena..";

    dialogue[2] =
        "Affan: Abey SHIT yaar! Main kese haar gaya?! Itni buri kismat bhi hoti hai kya? 😭";

    dialogue[3] =
        "Affan: Kher... ab to treat deni hi paray gi inko. Ludo bhi haar gaya aur paisay bhi gaye.";
    neonDialogue[0] =
        "Uncle: Beta, yahan protest ho raha hai, sab ek doosray ko maar rahe hain. Tumhein yahan nahi aana chahiye tha.";

    neonDialogue[1] =
        "Uncle: Yahan se nikalne ka sirf ek hi sahi darwaza hai. Baaki dono ghalat hain... aur asal chaabi sirf ek guard ke paas hai.";

    neonDialogue[2] =
        "Uncle: Ye apne paas rakho, hifazat ke liye. Sahi guard ko dhoondo aur sahi darwazay se yahan se nikal jao.";

    neonDialogue[3] =
        "Affan: Ye main kidhar phas gaya hoon?! Kya main sahi darwaza aur sahi chaabi dhoond paunga?";

    neonDialogue[4] =
        "Affan: Main yahan se nikal ke rahunga... jo marzi ho jaayeeeeeee!";
}


// ============================================================
// INITIALIZE
// ============================================================

bool Cutscene::Initialize(
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
)
{
    const char* imagePaths[5] =
    {
        image1Path,
        image2Path,
        image3Path,
        image4Path,
        image5Path
    };

    const char* dialoguePaths[5] =
    {
        dialogue1Path,
        dialogue2Path,
        dialogue3Path,
        dialogue4Path,
        dialogue5Path
    };


    bool success = true;


    // ========================================================
    // LOAD IMAGES
    // ========================================================

    for (int i = 0; i < 5; i++)
    {
        if (imagePaths[i] == nullptr)
        {
            TraceLog(
                LOG_ERROR,
                "Cutscene image path %d is null.",
                i + 1
            );

            success = false;
            continue;
        }


        images[i] = LoadTexture(imagePaths[i]);


        if (images[i].id == 0)
        {
            TraceLog(
                LOG_ERROR,
                "Failed to load cutscene image %d: %s",
                i + 1,
                imagePaths[i]
            );

            imagesLoaded[i] = false;
            success = false;
        }
        else
        {
            imagesLoaded[i] = true;

            SetTextureFilter(
                images[i],
                TEXTURE_FILTER_BILINEAR
            );
        }
    }


    // ========================================================
    // BACKGROUND MUSIC
    //
    // Loops continuously for the whole cutscene.
    // ========================================================

    if (bgMusicPath != nullptr)
    {
        bgMusic = LoadMusicStream(bgMusicPath);

        if (bgMusic.frameCount == 0)
        {
            TraceLog(
                LOG_WARNING,
                "Failed to load cutscene bg music: %s",
                bgMusicPath
            );

            bgMusicLoaded = false;
        }
        else
        {
            bgMusic.looping = true;
            bgMusicLoaded = true;
        }
    }


    // ========================================================
    // TYPING SOUND
    // ========================================================

    if (typingSoundPath != nullptr)
    {
        typingSound = LoadSound(typingSoundPath);

        if (typingSound.frameCount == 0)
        {
            TraceLog(
                LOG_WARNING,
                "Failed to load cutscene typing sound: %s",
                typingSoundPath
            );

            typingSoundLoaded = false;
        }
        else
        {
            typingSoundLoaded = true;
        }
    }


    // ========================================================
    // DIALOGUE VOICE-OVER (one per scene)
    // ========================================================

    for (int i = 0; i < 5; i++)
    {
        if (dialoguePaths[i] == nullptr)
        {
            dialogueSoundsLoaded[i] = false;
            continue;
        }

        dialogueSounds[i] = LoadSound(dialoguePaths[i]);

        if (dialogueSounds[i].frameCount == 0)
        {
            TraceLog(
                LOG_WARNING,
                "Failed to load cutscene dialogue sound %d: %s",
                i + 1,
                dialoguePaths[i]
            );

            dialogueSoundsLoaded[i] = false;
        }
        else
        {
            dialogueSoundsLoaded[i] = true;
        }
    }


    return success;
}


// ============================================================
// START
// ============================================================

void Cutscene::Start(bool neon)
{
    useNeonDialogue = neon;

    playing = true;
    finished = false;

    currentScene = 0;

    transitionTimer = 0.0f;

    transitioning = true;

    printedWords = 0;
    wordTimer = 0.0f;

    // Background music loops for the whole cutscene — started once
    // here, not restarted per scene.
    if (bgMusicLoaded)
    {
        PlayMusicStream(bgMusic);
    }

    PlayDialogueSound(currentScene);
}


// ============================================================
// START SCENE
// ============================================================

void Cutscene::StartScene(int scene)
{
    if (scene < 0 || scene >= 5)
        return;


    currentScene = scene;

    transitionTimer = 0.0f;

    transitioning = true;

    printedWords = 0;

    wordTimer = 0.0f;

    PlayDialogueSound(currentScene);
}


// ============================================================
// PLAY DIALOGUE SOUND
// ============================================================

void Cutscene::PlayDialogueSound(int scene)
{
    // Stop any dialogue line still playing from the previous scene.
    for (int i = 0; i < 5; i++)
    {
        if (dialogueSoundsLoaded[i] && IsSoundPlaying(dialogueSounds[i]))
        {
            StopSound(dialogueSounds[i]);
        }
    }

    if (scene >= 0 && scene < 5 && dialogueSoundsLoaded[scene])
    {
        PlaySound(dialogueSounds[scene]);
    }
}


// ============================================================
// UPDATE
// ============================================================

void Cutscene::Update()
{
    if (!playing)
        return;


    float dt = GetFrameTime();

    // Keep the bg music stream fed while the cutscene is playing.
    if (bgMusicLoaded)
    {
        UpdateMusicStream(bgMusic);
    }


    // ========================================================
    // TRANSITION TIMER
    // ========================================================

    if (transitioning)
    {
        transitionTimer += dt;


        if (transitionTimer >= transitionDuration)
        {
            transitionTimer = transitionDuration;

            transitioning = false;
        }
    }


    // ========================================================
    // WORD-BY-WORD DIALOGUE
    // ========================================================

    UpdateDialogue();


    // ========================================================
    // ENTER
    // ========================================================

    if (IsKeyPressed(KEY_ENTER))
    {
        // ====================================================
        // NEXT IMAGE
        // ====================================================

        if (useNeonDialogue)
        {
            if (currentScene < 4)
            {
                StartScene(currentScene + 1);

                return;
            }
        }
        else
        {
            if (currentScene < 3)
            {
                StartScene(currentScene + 1);

                return;
            }
        }


        // ====================================================
        // CUTSCENE FINISHED
        // ====================================================

        playing = false;

        finished = true;

        if (bgMusicLoaded && IsMusicStreamPlaying(bgMusic))
        {
            StopMusicStream(bgMusic);
        }

        for (int i = 0; i < 5; i++)
        {
            if (dialogueSoundsLoaded[i] && IsSoundPlaying(dialogueSounds[i]))
            {
                StopSound(dialogueSounds[i]);
            }
        }

        TraceLog(
            LOG_INFO,
            "Cutscene finished."
        );
    }
}


// ============================================================
// UPDATE DIALOGUE
// ============================================================

void Cutscene::UpdateDialogue()
{
    if (currentScene < 0 ||
        currentScene >= 5)
    {
        return;
    }


    const std::string& text =
        useNeonDialogue
        ? neonDialogue[currentScene]
        : dialogue[currentScene];


    if (text.empty())
        return;


    int totalWords = 0;

    SplitDialogueWords(
        text,
        totalWords
    );


    if (printedWords >= totalWords)
        return;


    wordTimer += GetFrameTime();


    if (wordTimer >= wordDelay)
    {
        wordTimer = 0.0f;

        printedWords++;

        // One typing blip per word revealed.
        if (typingSoundLoaded)
        {
            PlaySound(typingSound);
        }

        if (printedWords > totalWords)
        {
            printedWords = totalWords;
        }
    }
}


// ============================================================
// SPLIT WORDS
// ============================================================

void Cutscene::SplitDialogueWords(
    const std::string& text,
    int& wordCount
) const
{
    std::stringstream stream(text);

    std::string word;

    wordCount = 0;


    while (stream >> word)
    {
        wordCount++;
    }
}


// ============================================================
// DRAW
// ============================================================

void Cutscene::Draw()
{
    if (!playing)
        return;


    // ========================================================
    // BACKGROUND
    // ========================================================

    ClearBackground(BLACK);


    // ========================================================
    // IMAGE
    // ========================================================

    DrawSceneImage();


    // ========================================================
    // DIALOGUE
    // ========================================================

    DrawDialogueBox();


    // ========================================================
    // TRANSITION
    // ========================================================

    if (transitioning)
    {
        DrawTransition();
    }


    // ========================================================
    // ENTER HINT
    // ========================================================

    const char* hint =
        "PRESS ENTER";


    int hintSize = 20;

    int hintWidth =
        MeasureText(
            hint,
            hintSize
        );


    DrawText(
        hint,
        GetScreenWidth() -
        hintWidth -
        35,
        GetScreenHeight() - 35,
        hintSize,
        Fade(WHITE, 0.75f)
    );
}


// ============================================================
// DRAW SCENE IMAGE
// ============================================================

void Cutscene::DrawSceneImage()
{
    if (!imagesLoaded[currentScene])
        return;


    Texture2D texture =
        images[currentScene];


    float screenWidth =
        (float)GetScreenWidth();

    float screenHeight =
        (float)GetScreenHeight();


    float imageWidth =
        (float)texture.width;

    float imageHeight =
        (float)texture.height;


    // ========================================================
    // FIT IMAGE TO SCREEN
    // ========================================================

    float scaleX =
        screenWidth / imageWidth;

    float scaleY =
        screenHeight / imageHeight;


    float scale =
        (scaleX > scaleY)
        ? scaleX
        : scaleY;


    float drawWidth =
        imageWidth * scale;

    float drawHeight =
        imageHeight * scale;


    float x =
        (screenWidth - drawWidth) / 2.0f;

    float y =
        (screenHeight - drawHeight) / 2.0f;


    // ========================================================
    // IMAGE 3 CAMERA EFFECT
    // ========================================================

    if (currentScene == 2)
    {
        float progress =
            transitionTimer /
            transitionDuration;


        if (progress > 1.0f)
            progress = 1.0f;


        // Slow cinematic zoom
        float zoom =
            1.0f +
            progress * 0.08f;


        drawWidth *= zoom;

        drawHeight *= zoom;


        x =
            (screenWidth - drawWidth) / 2.0f;

        y =
            (screenHeight - drawHeight) / 2.0f;
    }


    Rectangle source =
    {
        0,
        0,
        (float)texture.width,
        (float)texture.height
    };


    Rectangle destination =
    {
        x,
        y,
        drawWidth,
        drawHeight
    };


    DrawTexturePro(
        texture,
        source,
        destination,
        { 0, 0 },
        0.0f,
        WHITE
    );
}


// ============================================================
// DRAW DIALOGUE BOX
// ============================================================

void Cutscene::DrawDialogueBox()
{
    if (currentScene < 0 ||
        currentScene >= 5)
    {
        return;
    }


    const std::string& text =
        useNeonDialogue
        ? neonDialogue[currentScene]
        : dialogue[currentScene];


    if (text.empty())
        return;


    // ========================================================
    // BOX ON RIGHT SIDE
    // ========================================================

    float boxWidth =
        GetScreenWidth() * 0.43f;

    float boxHeight =
        155.0f;


    float boxX =
        GetScreenWidth() -
        boxWidth -
        45.0f;


    float boxY =
        GetScreenHeight() -
        boxHeight -
        55.0f;


    Rectangle box =
    {
        boxX,
        boxY,
        boxWidth,
        boxHeight
    };


    // Dark cinematic background
    DrawRectangleRounded(
        box,
        0.06f,
        20,
        Color{
            3,
            8,
            15,
            225
        }
    );


    // Golden border
    DrawRectangleRoundedLinesEx(
        box,
        0.06f,
        20,
        3.0f,
        GOLD
    );


    // ========================================================
    // GET PRINTED WORDS
    // ========================================================

    std::stringstream stream(text);

    std::string word;

    std::string displayedText;


    int index = 0;


    while (stream >> word)
    {
        if (index >= printedWords)
            break;


        if (!displayedText.empty())
            displayedText += " ";


        displayedText += word;


        index++;
    }


    // ========================================================
    // DRAW WORD-BY-WORD TEXT
    // ========================================================

    int fontSize = 24;

    int padding = 22;


    // Basic word wrapping
    std::stringstream textStream(
        displayedText
    );


    std::string currentLine;

    int lineY =
        (int)(box.y + padding);


    while (textStream >> word)
    {
        std::string testLine =
            currentLine;


        if (!testLine.empty())
            testLine += " ";


        testLine += word;


        if (MeasureText(
            testLine.c_str(),
            fontSize
        ) > box.width - padding * 2)
        {
            DrawText(
                currentLine.c_str(),
                (int)(box.x + padding),
                lineY,
                fontSize,
                WHITE
            );


            lineY += 34;

            currentLine =
                word;
        }
        else
        {
            currentLine =
                testLine;
        }
    }


    if (!currentLine.empty())
    {
        DrawText(
            currentLine.c_str(),
            (int)(box.x + padding),
            lineY,
            fontSize,
            WHITE
        );
    }
}


// ============================================================
// TRANSITION
// ============================================================

void Cutscene::DrawTransition()
{
    switch (currentScene)
    {
    case 0:
        DrawFadeTransition();
        break;

    case 1:
        DrawSmoothTransition();
        break;

    case 2:
        DrawCameraTransition();
        break;

    case 3:
        DrawSmoothTransition();
        break;
    }
}


// ============================================================
// IMAGE 1 - FADE IN
// ============================================================

void Cutscene::DrawFadeTransition()
{
    float progress =
        transitionTimer /
        transitionDuration;


    if (progress > 1.0f)
        progress = 1.0f;


    float alpha =
        1.0f - progress;


    DrawRectangle(
        0,
        0,
        GetScreenWidth(),
        GetScreenHeight(),
        Fade(
            BLACK,
            alpha
        )
    );
}


// ============================================================
// IMAGE 2 / IMAGE 4 - SMOOTH
// ============================================================

void Cutscene::DrawSmoothTransition()
{
    float progress =
        transitionTimer /
        transitionDuration;


    if (progress > 1.0f)
        progress = 1.0f;


    float alpha =
        1.0f - progress;


    DrawRectangle(
        0,
        0,
        GetScreenWidth(),
        GetScreenHeight(),
        Fade(
            BLACK,
            alpha
        )
    );
}


// ============================================================
// IMAGE 3 - CAMERA EFFECT
// ============================================================

void Cutscene::DrawCameraTransition()
{
    float progress =
        transitionTimer /
        transitionDuration;


    if (progress > 1.0f)
        progress = 1.0f;


    // Cinematic black fade at beginning
    float alpha =
        0.55f *
        (1.0f - progress);


    DrawRectangle(
        0,
        0,
        GetScreenWidth(),
        GetScreenHeight(),
        Fade(
            BLACK,
            alpha
        )
    );


    // Cinematic letterbox
    int barHeight =
        (int)(55.0f * (1.0f - progress));


    if (barHeight > 0)
    {
        DrawRectangle(
            0,
            0,
            GetScreenWidth(),
            barHeight,
            BLACK
        );


        DrawRectangle(
            0,
            GetScreenHeight() - barHeight,
            GetScreenWidth(),
            barHeight,
            BLACK
        );
    }
}


// ============================================================
// STATE
// ============================================================

bool Cutscene::IsPlaying() const
{
    return playing;
}


bool Cutscene::IsFinished() const
{
    return finished;
}


// ============================================================
// UNLOAD
// ============================================================

void Cutscene::Unload()
{
    // ========================================================
    // TEXTURES
    // ========================================================

    for (int i = 0; i < 4; i++)
    {
        if (imagesLoaded[i])
        {
            UnloadTexture(
                images[i]
            );

            images[i] = {};

            imagesLoaded[i] = false;
        }
    }


    // ========================================================
    // AUDIO
    // ========================================================

    if (bgMusicLoaded)
    {
        UnloadMusicStream(bgMusic);
        bgMusicLoaded = false;
    }

    if (typingSoundLoaded)
    {
        UnloadSound(typingSound);
        typingSoundLoaded = false;
    }

    for (int i = 0; i < 5; i++)
    {
        if (dialogueSoundsLoaded[i])
        {
            UnloadSound(dialogueSounds[i]);
            dialogueSoundsLoaded[i] = false;
        }
    }


    playing = false;
}