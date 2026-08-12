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
        "Affan: 6 se maar isko, phir dobara baari le! Isko aaj bachne nahi dena.😭";

    dialogue[1] =
        "Rohaan: Chalo shart laga lo, jo haara woh sab ko Drumble aur AFC khilaye ga. Hassan: Done karo bhai!Aaj to jeet ke rahun ga, dekh lena..";

    dialogue[2] =
        "Affan: Abey SHIT yaar! Main kese haar gaya?! Itni buri kismat bhi hoti hai kya? 😭";

    dialogue[3] =
        "Affan: Kher... ab to treat deni hi paray gi inko. Ludo bhi haar gaya aur paisay bhi gaye.";
}


// ============================================================
// INITIALIZE
// ============================================================

bool Cutscene::Initialize(
    const char* image1Path,
    const char* image2Path,
    const char* image3Path,
    const char* image4Path,

    const char* audio1Path,
    const char* audio2Path,
    const char* audio3Path,
    const char* audio4Path
)
{
    const char* imagePaths[4] =
    {
        image1Path,
        image2Path,
        image3Path,
        image4Path
    };

    const char* audioPaths[4] =
    {
        audio1Path,
        audio2Path,
        audio3Path,
        audio4Path
    };


    bool success = true;


    // ========================================================
    // LOAD IMAGES
    // ========================================================

    for (int i = 0; i < 4; i++)
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
    // LOAD AUDIO
    // ========================================================

    for (int i = 0; i < 4; i++)
    {
        if (audioPaths[i] == nullptr)
        {
            TraceLog(
                LOG_WARNING,
                "Cutscene audio path %d is null.",
                i + 1
            );

            soundsLoaded[i] = false;
            continue;
        }


        sounds[i] = LoadSound(audioPaths[i]);


        if (sounds[i].frameCount == 0)
        {
            TraceLog(
                LOG_WARNING,
                "Failed to load cutscene audio %d: %s",
                i + 1,
                audioPaths[i]
            );

            soundsLoaded[i] = false;
        }
        else
        {
            soundsLoaded[i] = true;
        }
    }


    return success;
}


// ============================================================
// START
// ============================================================

void Cutscene::Start()
{
    playing = true;
    finished = false;

    currentScene = 0;

    transitionTimer = 0.0f;

    transitioning = true;

    printedWords = 0;
    wordTimer = 0.0f;

    StopCurrentAudio();


    // ========================================================
    // PLAY FIRST AUDIO
    // ========================================================

    if (soundsLoaded[0])
    {
        PlaySound(sounds[0]);
    }


    TraceLog(
        LOG_INFO,
        "Cutscene started."
    );
}


// ============================================================
// STOP AUDIO
// ============================================================

void Cutscene::StopCurrentAudio()
{
    if (currentScene >= 0 &&
        currentScene < 4 &&
        soundsLoaded[currentScene])
    {
        StopSound(
            sounds[currentScene]
        );
    }
}


// ============================================================
// START SCENE
// ============================================================

void Cutscene::StartScene(int scene)
{
    if (scene < 0 || scene >= 4)
        return;


    // Stop previous sound immediately
    StopCurrentAudio();


    currentScene = scene;

    transitionTimer = 0.0f;

    transitioning = true;

    printedWords = 0;

    wordTimer = 0.0f;


    // Start the new sound immediately
    if (soundsLoaded[currentScene])
    {
        PlaySound(
            sounds[currentScene]
        );
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
        // Stop current audio IMMEDIATELY
        StopCurrentAudio();


        // ====================================================
        // NEXT IMAGE
        // ====================================================

        if (currentScene < 3)
        {
            StartScene(
                currentScene + 1
            );

            return;
        }


        // ====================================================
        // CUTSCENE FINISHED
        // ====================================================

        playing = false;

        finished = true;

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
        currentScene >= 4)
    {
        return;
    }


    const std::string& text =
        dialogue[currentScene];


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
        currentScene >= 4)
    {
        return;
    }


    const std::string& text =
        dialogue[currentScene];


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
    StopCurrentAudio();


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

    for (int i = 0; i < 4; i++)
    {
        if (soundsLoaded[i])
        {
            UnloadSound(
                sounds[i]
            );

            sounds[i] = {};

            soundsLoaded[i] = false;
        }
    }


    playing = false;
}