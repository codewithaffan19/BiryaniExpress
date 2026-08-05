#include "Player.h"
#include "raymath.h"
#include "../Core/InputManager.h"
#include "../World/Collision.h"
#include "../World/Map.h"
#include <cmath>

Player::Player()
{
    position = { 1.5f,1.5f };
    direction = { 1.0f,0.0f };
    cameraPlane = { 0.0f,0.66f };
    moveSpeed = 3.0f;
    radius = 0.25f;
    Inventory.push_back(WEAPON_SPOON);
    currentWeaponIndex = 0;
}
WeaponType Player::ActivateWeapon()const {
    if (Inventory.empty())
        return WEAPON_SPOON;
    return Inventory[currentWeaponIndex];
}

void Player::Update(
    float dt,
    InputManager& input,
    Map& map)
{
    //Inventory
    if (IsKeyDown(KEY_ONE)&&Inventory.size()>=1) {
        currentWeaponIndex = 0;
    }
    if (IsKeyDown(KEY_TWO)&&Inventory.size()>=2) {
        currentWeaponIndex = 1;
    }
    if (IsKeyDown(KEY_THREE)&&Inventory.size()>=3) {
        currentWeaponIndex = 2;
    }

    bool isWalking = IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D);
    if (isWalking) {
        weaponbobtimer = dt * 10.0f;
    }
    else {
        weaponbobtimer = Lerp(weaponbobtimer, 0.0f, dt * 10.0f);
    }

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