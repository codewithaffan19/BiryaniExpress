#include "raylib.h"
#include "../World/Map.h"

//Objects 
enum { NOWALL, WALL };

Vector2 CheckMapCollosion(Vector2 pos, float PlayerRadius, Vector2 Velocity, Map& map) {
	Vector2 NewPos = pos;
	NewPos.x += Velocity.x;
	NewPos.y += Velocity.y;

	float checkX = NewPos.x;

	if (Velocity.x > 0)
		checkX += PlayerRadius;//right
	else
		checkX -= PlayerRadius;//left

	float checkY = NewPos.y;
	if (Velocity.y > 0)
		checkY += PlayerRadius;//right
	else
		checkY -= PlayerRadius;//left

	int gridX = static_cast<int>(checkX);
	int gridY = static_cast<int>(checkY);
	int currentGridX = static_cast<int>(pos.x);
	int currentGridY = static_cast<int>(pos.y);

	if (map.GetCell(gridX, currentGridY) == NOWALL) {
		pos.x = NewPos.x;
	}
	if (map.GetCell(currentGridX, gridY) == NOWALL) {
		pos.y = NewPos.y;
	}
	return pos;
}