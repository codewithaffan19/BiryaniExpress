#include "InputManager.h"

InputManager::InputManager()
{
    mouseDelta = { 0,0 };
}

void InputManager::Update()
{
    mouseDelta = ::GetMouseDelta();
}

bool InputManager::Forward() const
{
    return IsKeyDown(KEY_W);
}

bool InputManager::Backward() const
{
    return IsKeyDown(KEY_S);
}

bool InputManager::Left() const
{
    return IsKeyDown(KEY_A);
}

bool InputManager::Right() const
{
    return IsKeyDown(KEY_D);
}

bool InputManager::Sprint() const
{
    return IsKeyDown(KEY_LEFT_SHIFT);
}

bool InputManager::Exit() const
{
    return WindowShouldClose();
}

bool InputManager::IsKeyPressed(int key) const
{
    return ::IsKeyPressed(key);
}

Vector2 InputManager::GetMouseDelta() const
{
    return mouseDelta;
}