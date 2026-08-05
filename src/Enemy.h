#pragma once
#include "raylib.h"
#include "../src/Player/Player.h"
class Enemy:public Player {
private:
	int state;
	float collosionDistance;
	float AttackDistance;
public:
	Texture2D spriteSheet;
	int currentframe;
	int totalframes=5;
	float frametimer;
	float framespeed;
	Enemy();
	void update(Player P, Map& m1);
};