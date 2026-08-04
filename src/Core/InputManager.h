#pragma once

#include "raylib.h"

class InputManager
{
public:

    InputManager();

    void Update();

    bool Forward() const;
    bool Backward() const;
    bool Left() const;
    bool Right() const;

    bool Sprint() const;

    bool Exit() const;

    bool IsKeyPressed(int key) const;

    Vector2 GetMouseDelta() const;

private:

    Vector2 mouseDelta;
};