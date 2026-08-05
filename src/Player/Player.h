#pragma once

#include "raylib.h"
#include "../World/Map.h"
#include <vector>
class InputManager;
class Map;

enum WeaponType {
    WEAPON_SPOON,WEAPON_CHAPPAL
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
    WeaponType ActivateWeapon()const;
    float weaponbobtimer;
    float moveSpeed;
    float radius;
    Vector2 position;
    std::vector<WeaponType>Inventory;
    int currentWeaponIndex;
    Texture2D spoonTex;
    Texture2D chappaltex;
private:
    Vector2 direction;
    Vector2 cameraPlane;
};