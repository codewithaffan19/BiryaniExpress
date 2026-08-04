#pragma once

#include "raylib.h"
#include "../World/Map.h"
class InputManager;
class Map;
class Player{
public:

    Player();

    void Update(
        float dt,
        InputManager& input,
        Map& map);


    Vector2 GetDirection() const;

    Vector2 GetCameraPlane() const;
    float moveSpeed;
    float radius;
    Vector2 position;
private:
    Vector2 direction;

    Vector2 cameraPlane;
};