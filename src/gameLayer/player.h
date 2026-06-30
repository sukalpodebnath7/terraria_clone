#pragma once 
#include <raylib.h>
#include <raymath.h>
#include <physics.h>


struct PlayerEntity : public PhysicalEntity {

	enum {
		standing = 0,
		moving,
		jumping,
		standingWithBlock,
		movingWithBlock,
		attacking
	};

	Texture2D playerTex;

	int state = 0;
	//int prevState = 0;
	bool direction = 1; // 1 -> Right and 0 -> left
	void entityAnimation();
	float playerWidth = 32; // pixels
	float playerHeight = 64; // pixels
	int selectedBlock = 0;
	float health[5] = { 20.f, 20.f, 20.f, 20.f, 20.f };
	float lastDownTouch;
	float takenDamage = 0.f;

	void updateHealth();
};