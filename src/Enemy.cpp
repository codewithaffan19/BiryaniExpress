#include "Enemy.h"
#include "../src/Player/Player.h"
#include "raylib.h"
#include "raymath.h"
#include "../src/World/Collision.h"

enum { STILL, CHASE,INTERROGATE,DEAD,ATTACK,HIT_REACT };

Enemy::Enemy() {
	position = { 5.5f,5.5f };
	radius = 0.25f;
	collosionDistance = 5.0f;
	AttackDistance = 1.0f;
	state = STILL;
	moveSpeed = 0.5f;
	currentframe = 0;
	framespeed = 0.5f;
	frametimer = 0.0f;
	health = 100;
	rows = 2;
	columns = 5;
}

void Enemy::update(Player& P, Map& m1) {
	float deltaTime = GetFrameTime();
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
	for (int i = 0; i < MAX_BUBBLES; i++) {
		if (bubbles[i].active) {
			bubbles[i].position.x += bubbles[i].velocity.x * deltaTime;
			bubbles[i].position.y += bubbles[i].velocity.y * deltaTime;

			bubbles[i].life -= deltaTime * 2.0f;
			if (bubbles->life <= 0.0f) {
				bubbles[i].active = false;
			}
		}
	}
	if (Respawn==true&&state==DEAD) {
		RespawnCheck += deltaTime;
		if (RespawnCheck >= RespawnTimer) {
			RespawnCheck = 0.0f;
			state = STILL;
			health = 100;
		}
	}
	if (health <= 0) {
		state = DEAD;
		Velocity = { 0.0f,0.0f };
		currentframe = 9;
		if (!hasPopped) {
			for (int i = 0; i < MAX_BUBBLES; i++) {
				bubbles[i].active = true;
				bubbles[i].position = position;

				bubbles[i].velocity.x = (float)GetRandomValue(-15, 15) / 10.0f;
				bubbles[i].velocity.y = (float)GetRandomValue(-15, 15) / 10.0f;
				bubbles[i].radius = (GetRandomValue(2, 6));
				bubbles[i].life = 1.0f;
			}
			hasPopped = true;
		}
		return;
	}
	else {
		state = CHASE;
	}

	if (KnockBackTimer > 0.0f) {
		position = KnockBack(position, KnockBackVelocity, KnockBackTimer, radius, m1, deltaTime);
		currentframe = (KnockBackTimer > 0.1f) ? 8 : 7;
		TraceLog(LOG_INFO, "Knockback active, timer=%f", KnockBackTimer);
	}
	else {
		if (hit.distance < playerdis) {
			if (state == CHASE) {
				state = INTERROGATE;
				WanderTimer = 0.0f;
			}
		}
		else if (playerdis <= attackRange) {
			state = ATTACK;
		}
		else {
			state = CHASE;
		}

		if (state == CHASE) {
			Velocity = { moveSpeed * NormalDis.x * deltaTime,moveSpeed * NormalDis.y * deltaTime };
			position = CheckMapCollosion(position, radius, Velocity, m1);
			LastKnownPlayerpos = P.position;
			ReachedLKP = false;
			frametimer += deltaTime;
			if (frametimer >= framespeed) {
				frametimer = 0.0f;
				currentframe++;
				if (currentframe >= columns) {
					currentframe = 0;
				}
			}
		}
		else if (state == STILL) {
			currentframe = 0;
		}
		else if (state == INTERROGATE)
		{
			WanderTimer -= deltaTime;
			if (!ReachedLKP) {
				float dx = LastKnownPlayerpos.x - position.x;
				float dy = LastKnownPlayerpos.y - position.y;
				float dis = sqrt((dx * dx) + (dy * dy));

				if (dis <= 0.5f) {
					ReachedLKP = true;
					WanderTimer = 0.0f;
				}
				else {
					float normX = dx / dis;
					float normY = dy / dis;
					Vector2 Velocity = { moveSpeed * normX * deltaTime, moveSpeed * normY * deltaTime };

					position = CheckMapCollosion(position, radius, Velocity, m1);
				}
			}
			else {
				if (WanderTimer <= 0.0f) {
					float randomAngle = GetRandomValue(0, 360) * DEG2RAD;
					WanderDir.x = cos(randomAngle);
					WanderDir.y = sin(randomAngle);
					WanderTimer = 0.5f;
				}
				float searchspeed = moveSpeed;
				Vector2 Velocity = { searchspeed * WanderDir.x,searchspeed * WanderDir.y };
				Vector2 Oldpos = position;
				position = CheckMapCollosion(position, radius, Velocity, m1);

				if (position.x == Oldpos.x && position.y == Oldpos.y) {
					WanderTimer = 0.0f;
				}
				frametimer += deltaTime;
				if (frametimer >= framespeed) {
					frametimer = 0.0f;
					currentframe++;
					if (currentframe >= columns)
						currentframe = 0;
				}
			}
		}
		else if (state == ATTACK) {
			Velocity = { 0.0f, 0.0f };
			currentAttackTimer -= deltaTime;


			float attackProgress = 1.0f - (currentAttackTimer / attackCooldown);
			if (attackProgress < 0.5f)
				currentframe = 5;
			else
				currentframe = 6;

			if (currentAttackTimer <= 0.0f) {
				P.health -= attackDamage;
				P.hitmessagetimer = 30;
				if (P.health < 0) P.health = 0;
				float dx = P.position.x - position.x;
				float dy = P.position.y - position.y;
				float dis = sqrt((dx * dx) + (dy * dy));
				if (dis > 0.0001) {
					float KnockBackForce = 4.0f;
					float nx = dx / dis;
					float ny = dy / dis;
					P.KnockBackVelocity = { nx * KnockBackForce,ny * KnockBackForce };
					P.KnockBackTimer = 0.2f;
				}
				hitReactionTimer = 0.3f;
				state = HIT_REACT;
				currentAttackTimer = attackCooldown;
			}
		}
		else if (state == HIT_REACT) {
			Velocity = { 0.0f, 0.0f };
			hitReactionTimer -= deltaTime;

			currentframe = (hitReactionTimer > 0.15f) ? 7 : 8;

			if (hitReactionTimer <= 0.0f) {
				state = CHASE;
			}
		}
	}
}