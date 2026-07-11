#include <items.h>
#include <iostream>

void Item::dropPrint(float deltaTime, Texture2D tex) {
	updateForces(deltaTime);
	checkCollisionOnce(*gameMap, transform.pos);
	updateFinal();
	int t = type;
	if (type >= 70) t -= 70;
	DrawTexturePro(
		tex,
		{ (float)t * 32, (float)4 * 32, (float)32, (float)32 },
		{ transform.getTopLeft().x, transform.getTopLeft().y, 1, 1},
		{ 0.f, 0.f },
		0.f,
		WHITE
	);
}