#include "Player.h"

#include "../Core/InputManager.h"
#include "../World/Collision.h"
#include "../World/Map.h"
#include <cmath>

Player::Player()
{
    position = { 5.5f,5.5f };

    direction = { 1.0f,0.0f };

    cameraPlane = { 0.0f,0.66f };

    moveSpeed = 3.0f;

    radius = 0.25f;
}

void Player::Update(
    float dt,
    InputManager& input,
    Map& map)
{
    //-------------------------------------------------
    // Mouse Look
    //-------------------------------------------------

    Vector2 mouse = input.GetMouseDelta();

    float sensitivity = 0.0025f;

    float angle = mouse.x * sensitivity;

    float oldDirX = direction.x;

    direction.x =
        direction.x * cosf(angle) -
        direction.y * sinf(angle);

    direction.y =
        oldDirX * sinf(angle) +
        direction.y * cosf(angle);

    float oldPlaneX = cameraPlane.x;

    cameraPlane.x =
        cameraPlane.x * cosf(angle) -
        cameraPlane.y * sinf(angle);

    cameraPlane.y =
        oldPlaneX * sinf(angle) +
        cameraPlane.y * cosf(angle);

    //-------------------------------------------------
    // Movement
    //-------------------------------------------------

    Vector2 newPos = position;

    if (input.Forward())
    {
        newPos.x += direction.x * moveSpeed * dt;
        newPos.y += direction.y * moveSpeed * dt;
    }

    if (input.Backward())
    {
        newPos.x -= direction.x * moveSpeed * dt;
        newPos.y -= direction.y * moveSpeed * dt;
    }

    Vector2 right =
    {
        -direction.y,
         direction.x
    };

    if (input.Left())
    {
        newPos.x -= right.x * moveSpeed * dt;
        newPos.y -= right.y * moveSpeed * dt;
    }

    if (input.Right())
    {
        newPos.x += right.x * moveSpeed * dt;
        newPos.y += right.y * moveSpeed * dt;
    }

    //-------------------------------------------------
    // Collision
    //-------------------------------------------------

    float radius = 0.20f;

    if (map.GetCell(
        (int)position.y,
        (int)(newPos.x + radius)) == 0 &&
        map.GetCell(
            (int)position.y,
            (int)(newPos.x - radius)) == 0)
    {
        position.x = newPos.x;
    }

    if (map.GetCell(
        (int)(newPos.y + radius),
        (int)position.x) == 0 &&
        map.GetCell(
            (int)(newPos.y - radius),
            (int)position.x) == 0)
    {
        position.y = newPos.y;
    }
}


Vector2 Player::GetDirection() const
{
    return direction;
}

Vector2 Player::GetCameraPlane() const
{
    return cameraPlane;
}