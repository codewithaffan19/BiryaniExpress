#pragma once

#include "raylib.h"
#include "../World/Map.h"
#include <vector>
class InputManager;
class Map;

enum ItemID {
    ITEM_EMPTY = 0,
    ITEM_SPOON = 1,
    ITEM_CHAPPAL = 2,
    ITEM_BIRYANI = 3,
    ITEM_DRUMBLE = 4,
    ITEM_AFC = 5,
    ITEM_MIKE=6,
    ITEM_CLINIX=7,
};

struct InventorySlot {
    int id = ITEM_EMPTY;
};

class Player{
public:

    Player();

    void Update(
        float dt,
        InputManager& input,
        Map& map);


    Vector2 GetDirection() const;
    Vector2 GetCameraPlane() const;
    int health=100;
    int maxhealth = 100;
    float weaponbobtimer;
    float moveSpeed;
    float radius;
    Vector2 position;
    int currentWeaponIndex;
    int hitmessagetimer;
    //
    Texture2D handTex;
    Texture2D currentTex;
    Texture2D Weapon1Tex;
    Texture2D MikeHandTex;
    
    Vector2 KnockBackVelocity = { 0.0f,0.0f };
    float KnockBackTimer=0.0f;
    int GetActiveWeapon()const;
    Vector2 GetPosition() const;
    void SwitchWeapon();
    bool PickUpItem(int newItem);
    //

    InventorySlot MissionPouch[3];
    InventorySlot WeaponPouch[3];

    //
    int MissionCount[2] = { 0,0 };
    int MissionTarget[2] = { 1,1 };
    int WeaponCount[3] = { 0,0,0 };
    int WeaponTarget[3] = { 1,1,1 };

    //
    int weaponTotalFrames=2;
    int weaponCurrentFrame=0;
    float weaponFrameTimer = 0.0f;
    float weaponFrameSpeed=0.3f;
    int weaponColumns = 2;
    int weaponRows = 1;
    bool isAttacking;
    //
    bool isPoliceTriggered=false;
private:
    Vector2 direction;
    Vector2 cameraPlane;
};