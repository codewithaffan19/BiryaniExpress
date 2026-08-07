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
void CheckPlayerEnemyCollision(Player& p, Enemy& E) {
	float dx = E.position.x-p.position.x;
	float dy =  E.position.y- p.position.y;
	float dis = sqrt((dx * dx) + (dy * dy));
	float RadiiSum = p.radius + E.radius;
	
	if (dis>0.0001f&& dis<RadiiSum) {
		float Overlap = RadiiSum - dis;
		float pushforce = Overlap / 2.0f;
		float NormalX = dx / dis;
		float NormalY = dy / dis;

		//Enemy Bump
		E.position.x = E.position.x + (NormalX * pushforce);
		E.position.y = E.position.y + (NormalY * pushforce);
		//PlayerBump
		p.position.x -= (NormalX * pushforce);
		p.position.y -= (NormalX * pushforce);
	}
}