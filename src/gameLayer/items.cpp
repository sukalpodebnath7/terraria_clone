#include <items.h>
#include <iostream>

void Item::dropPrint(float deltaTime, Texture2D tex) {
	updateForces(deltaTime);
	checkCollisionOnce(*gameMap, transform.pos);
	updateFinal();
	DrawTexturePro(
		tex,
		{ (float)type * 32, (float)4 * 32, (float)32, (float)32 },
		{ transform.getTopLeft().x, transform.getTopLeft().y, 1, 1},
		{ 0.f, 0.f },
		0.f,
		WHITE
	);
}