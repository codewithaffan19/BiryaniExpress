#include "Enemy.h"
#include "../src/Player/Player.h"
#include "raylib.h"
#include "raymath.h"
#include "../src/World/Collision.h"

enum { STILL, CHASE, ATTACK,STUNNED };

Enemy::Enemy() {
	position = { 5.5f,5.5f };
	radius = 0.4f;
	collosionDistance = 5.0f;
	AttackDistance = 1.0f;
	state = STILL;
	moveSpeed = 0.5f;
	currentframe = 0;
	framespeed = 0.15f;
	frametimer = 0.0f;		
}

void Enemy::update(Player P, Map& m1) {
	float dx = P.position.x - position.x;
	float dy = P.position.y - position.y;

	float playerdis = sqrt(dx * dx + dy * dy);
	Vector2 NormalDis;
	if (playerdis > 0) {
		NormalDis = { dx / playerdis,dy / playerdis };
	}
	Vector2 Velocity;
	int side = 0;
	int sideHit = 0;
	RayHit hit = m1.CastSingleRay(position, NormalDis, m1, side);
	float WallDistance = hit.distance;
	float deltaTime = GetFrameTime();
	if (WallDistance < playerdis) {
		state = STILL;
	}
	if (WallDistance > playerdis) {
		state = CHASE;
	}

	if (state == CHASE) {
		Velocity = { moveSpeed * NormalDis.x * deltaTime,moveSpeed * NormalDis.y * deltaTime };
		position = CheckMapCollosion(position, radius, Velocity, m1);
	}
	frametimer += deltaTime;
	if (frametimer >= framespeed) {
		frametimer = 0.0f;
		currentframe++;
		if (currentframe >= totalframes) {
			currentframe = 0;
		}
	}
}