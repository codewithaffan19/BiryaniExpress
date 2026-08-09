#pragma once
#include "raylib.h"
#include"Map.h"
#include "../Player/Player.h"
#include "../Enemy.h"
#include "../NPC/NPC.h"
//Removed static keyword because it has different meaning outside the class:Keep it private outside of this cpp where it is defined
Vector2 CheckMapCollosion(Vector2 pos, float PlayerRadius, Vector2 Velocity, Map& map);
void CheckPlayerEnemyCollision(Player& p, Enemy&E,Map&m1);
void CheckPlayerNPCCollision(Player& E, NPC&p,Map&m1);