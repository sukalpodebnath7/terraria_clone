#include <player.h>


void PlayerEntity::entityAnimation() {
	static int  prevState = 0;
	static int pos = 0;
	static int frameCounter = 0;

	frameCounter++;

	if (frameCounter >= 24)
	{
		pos = (pos + 1) % 7;
		frameCounter = 0;
	}

	//if (prevState == jumping && state == moving) {}
	if (state != prevState) {
		prevState = state;
		pos = 0;
	}

	int posMultiple = 32;
	float widthStart = 0;
	float widthRange = 32;

	if (!direction) { //player is left faced
		widthStart = 32;
		widthRange *= (-1);
	}
	
	DrawTexturePro(
		playerTex,
		{ (float)widthStart + playerWidth * pos, (float)state*playerHeight, widthRange, playerHeight },
		{ transform.getTopLeft().x, transform.getTopLeft().y, 0.9f, 1.8f },
		{ 0,0 },
		0.f,
		WHITE
	);
}


void PlayerEntity::updateHealth() {
	if (takenDamage == 0.f) return;

	for (int i = 0; i < 5; i++) {
		if (health[i] - takenDamage >= 0.f) {
			health[i] -= takenDamage;
			takenDamage = 0.f;
			break;
		}

		else if (health[i] != 0) {
			takenDamage -= health[i];
			health[i] = 0.f;
		}
	}
	
}