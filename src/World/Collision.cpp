#include "raylib.h"
#include "../World/Map.h"
#include "../Player/Player.h"
#include "../Enemy.h"
#include "../NPC/NPC.h"
#include <vector>
//Objects 
enum { NOWALL, WALL };
enum { STILL, CHASE, INTERROGATE, DEAD, ATTACK, HIT_REACT };//states
Vector2 CheckMapCollosion(Vector2 pos, float PlayerRadius, Vector2 Velocity, Map& map) {
	Vector2 NewPos = pos;
	NewPos.x += Velocity.x;
	NewPos.y += Velocity.y;

	float checkX = NewPos.x;
	if (Velocity.x > 0)
		checkX += PlayerRadius;
	else
		checkX -= PlayerRadius;

	float checkY = NewPos.y;
	if (Velocity.y > 0)
		checkY += PlayerRadius;
	else
		checkY -= PlayerRadius;

	int gridX = static_cast<int>(checkX);
	int gridY = static_cast<int>(checkY);
	int currentGridX = static_cast<int>(pos.x);
	int currentGridY = static_cast<int>(pos.y);

	int tileX = map.GetCell(currentGridY, gridX);
	int tileY = map.GetCell(gridY, currentGridX);

	if (tileX == NOWALL) {
		pos.x = NewPos.x;
	}
	if (tileY == NOWALL) {
		pos.y = NewPos.y;
	}

	return pos;
}
void CheckPlayerEnemyCollision(Player& p, Enemy& E,Map&m1) {
	float dx = E.position.x-p.position.x;
	float dy =  E.position.y- p.position.y;
	float dis = sqrt((dx * dx) + (dy * dy));
	float RadiiSum = p.radius + E.radius;
	
	if (dis>0.0001f&& dis<RadiiSum) {
		float Overlap = RadiiSum - dis;
		float pushforce = Overlap / 2.0f;
		float NormalX = dx / dis;
		float NormalY = dy / dis;
		Vector2 PlayerBumpVelocity = { -NormalX * pushforce, -NormalY * pushforce };

		Vector2 OldPlayerPos = p.position;
		p.position = CheckMapCollosion(p.position, p.radius, PlayerBumpVelocity, m1);

		float movedX = p.position.x - OldPlayerPos.x;
		float movedY = p.position.y - OldPlayerPos.y;
		float actualMovedDis = sqrt((movedX * movedX) + (movedY * movedY));
		//Moveable distance check
		float EnemyForce = pushforce + (pushforce - actualMovedDis);

		Vector2 EnemyBumpVelocity = { NormalX * EnemyForce, NormalY * EnemyForce };
		E.position = CheckMapCollosion(E.position, E.radius, EnemyBumpVelocity, m1);
	}
}
void CheckPlayerNPCCollision(Player& E, NPC& p, Map& m1) {
	float dx = E.position.x - p.GetPosition().x;
	float dy = E.position.y - p.GetPosition().y;
	float dis = sqrt((dx * dx) + (dy * dy));
	float RadiiSum = E.radius;

	if (dis > 0.0001f && dis < RadiiSum) {
		float Overlap = RadiiSum - dis;
		float pushforce = Overlap / 2.0f;
		float NormalX = dx / dis;
		float NormalY = dy / dis;
		Vector2 PlayerBumpVelocity = { -NormalX * pushforce, -NormalY * pushforce };

		Vector2 OldPlayerPos = p.GetPosition();
		p.GetPosition() = CheckMapCollosion(p.GetPosition(), p.radius, PlayerBumpVelocity, m1);

		float movedX = p.GetPosition().x - OldPlayerPos.x;
		float movedY = p.GetPosition().y - OldPlayerPos.y;
		float actualMovedDis = sqrt((movedX * movedX) + (movedY * movedY));
		//Moveable distance check
		float EnemyForce = pushforce + (pushforce - actualMovedDis);

		Vector2 EnemyBumpVelocity = { NormalX * EnemyForce, NormalY * EnemyForce };
		E.position = CheckMapCollosion(E.position, E.radius, EnemyBumpVelocity, m1);
	}
}
void CheckEnemyEnemyCollision(std::vector<Enemy>&enemies ,Map& m1) {
	for (size_t i = 0; i < enemies.size(); i++) {
		if (enemies[i].state == DEAD) {
			continue;
		}
		for (size_t j = i + 1; j < enemies.size();j++) {
			if (enemies[j].state == DEAD)
				continue;
			Vector2 a = enemies[i].position;
			Vector2 b = enemies[j].position;
			float dx = b.x - a.x;
			float dy = b.y - a.y;
			float dis = sqrt((dx * dx) + (dy * dy));
			float minimumdist = enemies[i].radius + enemies[j].radius;
			if (dis > 0.0001f && dis < minimumdist) {
				float overlap = minimumdist - dis;

				float nx = dx / dis;
				float ny = dy / dis;
				float pushx = nx * overlap * 0.5f;
				float pushy = ny * overlap * 0.5f;

				Vector2 NewposA = { a.x - pushx,a.y - pushy };
				Vector2 NewposB = { b.x + pushx,b.y + pushy };

				enemies[i].position = CheckMapCollosion(a, enemies[i].radius, { NewposA.x - a.x,NewposA.y - a.y }, m1);
				enemies[j].position = CheckMapCollosion(b, enemies[j].radius, { NewposB.x - b.x,NewposB.y - b.y }, m1);
 			}
			else if (dis <= 0.00001f) {
				enemies[i].position.x -= 0.01f;
				enemies[j].position.x += 0.01f;
			}
		}
	}
}

Vector2 KnockBack(Vector2 pos, Vector2& KnockBackVelocity, float &KnockBackTimer, float radius, Map& m1, float dt) {
	if (KnockBackTimer <= 0) {
		return pos;
	}
	KnockBackTimer -= dt;
	Vector2 MoveDifference = { KnockBackVelocity.x * dt,KnockBackVelocity.y * dt };
	pos = CheckMapCollosion(pos, radius, MoveDifference, m1);

	float damping = 6.0f;
	KnockBackVelocity.x -= KnockBackVelocity.x * damping * dt;
	KnockBackVelocity.y -= KnockBackVelocity.y * damping * dt;

	if (KnockBackTimer <= 0.0f) {
		KnockBackVelocity = { 0.0f,0.0f };
	}
	return pos;
}
