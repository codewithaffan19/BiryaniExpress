#include "Player.h"
#include "raymath.h"
#include "../Core/InputManager.h"
#include "../World/Collision.h"
#include "../World/Map.h"
#include <cmath>

Player::Player()
{
    position = { 2.0f,2.0f };
    direction = { 1.0f,0.0f };
    cameraPlane = { 0.0f,0.66f };
    moveSpeed = 3.0f;
    radius = 0.25f;
    currentWeaponIndex = 0;
    hitmessagetimer = 0;
    WeaponPouch[0].id = ITEM_SPOON;
    WeaponPouch[1].id = ITEM_CHAPPAL;
}
int Player::GetActiveWeapon()const {
    return WeaponPouch[currentWeaponIndex].id;
}
void Player::SwitchWeapon(){
    int nextWeapon;
    if (currentWeaponIndex == 0)
        nextWeapon = 1;
    else
        nextWeapon = 0;
    if (WeaponPouch[nextWeapon].id != ITEM_EMPTY) {
        currentWeaponIndex = nextWeapon;
    }
}

bool Player::PickUpItem(int newItem) {

    if (newItem == ITEM_BIRYANI || newItem == ITEM_DRUMBLE || newItem == ITEM_AFC) {
        for (int i = 0; i < 3; i++) {
            if (MissionPouch[i].id == ITEM_EMPTY){
                MissionPouch[i].id = newItem;
                return true;
        }
        }
        return false;
    }
    if (newItem == ITEM_CHAPPAL || newItem == ITEM_SPOON) {
        for (int i = 0; i < 2; i++) {
            if (WeaponPouch[i].id == ITEM_EMPTY) {
                WeaponPouch[i].id = newItem;
                return true;
            }
        }

        return false;
    }   

    return false;
}
void Player::Update(
    float dt,
    InputManager& input,
    Map& map)
{
    //Inventory
    if (IsKeyPressed(KEY_ONE)&&WeaponPouch[0].id!=ITEM_EMPTY) {
        SwitchWeapon();
    }
    else if (IsKeyPressed(KEY_TWO)&& WeaponPouch[1].id != ITEM_EMPTY) {
        SwitchWeapon();
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
    if (hitmessagetimer > 0)
        hitmessagetimer--;
}


Vector2 Player::GetDirection() const
{
    return direction;
}

Vector2 Player::GetCameraPlane() const
{
    return cameraPlane;
}
Vector2 Player::GetPosition() const
{
    return position;
}