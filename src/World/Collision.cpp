#include "raylib.h"
#include "../World/Map.h"
#include "../Player/Player.h"
#include "../Enemy.h"
//Objects 
enum { NOWALL, WALL };
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