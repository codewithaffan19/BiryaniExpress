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
    WeaponPouch[0].id = ITEM_BIRYANI;
    WeaponPouch[1].id = ITEM_EMPTY;
    WeaponPouch[2].id = ITEM_EMPTY;

}
int Player::GetActiveWeapon()const {
    return WeaponPouch[currentWeaponIndex].id;
}
void Player::SwitchWeapon() {
    int nextWeapon = 0;

    if (currentWeaponIndex == 0) {
        nextWeapon = 1;
    }
    else if (currentWeaponIndex == 1) {
        nextWeapon = 2;
    }
    else if (currentWeaponIndex == 2) {
        nextWeapon = 0;
    }

    currentWeaponIndex = nextWeapon;

    if (WeaponPouch[nextWeapon].id == ITEM_EMPTY) {
        currentTex = handTex;
        weaponTotalFrames = 2;
    }
    else if (WeaponPouch[nextWeapon].id == ITEM_MIKE) {
        currentTex = MikeHandTex;
        weaponTotalFrames = 2;
    }
    else if (WeaponPouch[nextWeapon].id == ITEM_SPOON) {
        currentTex = Weapon1Tex;
        weaponTotalFrames = 2;
    }
    else {
        currentTex = handTex;
        weaponTotalFrames = 2;
    }

    weaponCurrentFrame = 0;
}

bool Player::PickUpItem(int newItem) {
    int index = 0;
    if (newItem == ITEM_BIRYANI || newItem == ITEM_DRUMBLE || newItem == ITEM_AFC) {
        if (newItem == ITEM_DRUMBLE) index = 0;
        else if (newItem == ITEM_AFC) index = 1;

        MissionPouch[index].id = newItem;
        MissionCount[index]++;
        if (MissionCount[index] > MissionTarget[index]) {
            MissionCount[index] = MissionTarget[index];
            return false;
        }
        return true;
    }
    index = 0;
    if (newItem == ITEM_MIKE || newItem == ITEM_SPOON) {
        if (newItem == ITEM_MIKE)index = 1;
        else if (newItem == ITEM_SPOON)index = 2;

        WeaponPouch[index].id = newItem;
        WeaponCount[index]++;
        if (WeaponCount[index] > MissionTarget[index]) {
            WeaponCount[index] = WeaponTarget[index];
            return false;
        }
        return true;
    }

    return false;
}
void Player::Update(
    float dt,
    InputManager& input,
    Map& map)
{
    /* if (health <= 0)
     {
         return;
     }*/
     //Inventory


    bool isWalking = IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D);
    isMoving = isWalking;
    if (IsKeyPressed(KEY_ONE) && WeaponPouch[0].id != ITEM_EMPTY) {
        SwitchWeapon();
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && weaponTotalFrames > 1) {
        isAttacking = true;
        weaponCurrentFrame = 1;
        weaponFrameTimer = 0.0f;
    }
    if (isAttacking) {
        weaponFrameTimer += dt;
        if (weaponFrameTimer >= weaponFrameSpeed) {
            weaponCurrentFrame = 0;
            weaponFrameTimer = 0.0f;
            isAttacking = false;
        }
    }
    if (isWalking && !isAttacking)
    {
        weaponCurrentFrame = 0;
    }
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
    if (KnockBackTimer > 0.0) {
        position = KnockBack(position, KnockBackVelocity, KnockBackTimer, radius, map, dt);
    }
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
bool Player::IsMoving() const
{
    return isMoving;
}
Vector2 Player::GetPosition() const
{
    return position;
}