#pragma once

#include "raylib.h"
#include "../World/Map.h"
class InputManager;
class Map;

class Player
{
public:

    Player();

    void Update(
        float dt,
        InputManager& input,
        Map& map);

    Vector2 GetPosition() const;

    Vector2 GetDirection() const;

    Vector2 GetCameraPlane() const;

private:

    Vector2 position;

    Vector2 direction;

    Vector2 cameraPlane;

    float moveSpeed;

    float radius;
};