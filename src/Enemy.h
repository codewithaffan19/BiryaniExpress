#pragma once
#include "raylib.h"
#include "../src/Player/Player.h"
class Enemy:public Player {
private:
	int state;
	float collosionDistance;
	float AttackDistance;
public:
	Texture texture;
	Enemy();
	void update(Player P, Map& m1);
};