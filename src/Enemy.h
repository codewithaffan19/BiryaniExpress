#pragma once
#include "raylib.h"
#include "../src/Player/Player.h"
struct Bubble {
	Vector2 position;
	Vector2 velocity;
	float radius;
	float life;
	bool active;
};
class Enemy:public Player {
private:
	
	float collosionDistance;
	float AttackDistance;
public:
	int state;
	Texture2D spriteSheet;
	int currentframe;
	int totalframes;
	Vector2 WanderDir = { 0.0f,0.0f };
	float WanderTimer;
	float frametimer;
	Vector2 LastKnownPlayerpos;
	bool ReachedLKP;
	float framespeed;
	float health;
	static const int MAX_BUBBLES = 12; 
	Bubble bubbles[MAX_BUBBLES];
	bool hasPopped = false;
	Enemy();
	void update(Player P, Map& m1);
};