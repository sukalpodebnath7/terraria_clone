#pragma once
#include <raylib.h>
#include <physics.h>
#include <gameMap.h>
#include <iostream>
using namespace std;

struct Entity : public PhysicalEntity {
	
	Entity(GameMap& gm)
		: gameMap(gm)
	{
	}

	virtual ~Entity() = default;

	enum {
		standing = 0,
		moving,
		jumping,
		attacking
	};

	Texture2D entityTex;

	bool playerDetected = 0;
	int state = 0;
	//int prevState = 0;
	int direction = 1;

	inline void entityAnimation() {
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

		if (direction == -1) { //player is left faced
			widthStart = 32;
			widthRange *= (-1);
		}

		DrawTexturePro(
			entityTex,
			{ (float)widthStart + entityWidth * pos, (float)state * entityHeight, widthRange, entityHeight },
			{ transform.getTopLeft().x, transform.getTopLeft().y, 0.9f, 1.8f },
			{ 0,0 },
			0.f,
			WHITE
		);
	}


	virtual void entityBehaviour(Transform2D& playerTransform) = 0;
	virtual void entityAction(Transform2D& player) = 0;

	inline bool playerDetectionCheck(Transform2D& player) {
		
		float delX = abs(transform.pos.x - player.pos.x);
		float delY = abs(transform.pos.y - player.pos.y);

		if (delX <= 20.f && delY <= 20.f)
			return true;


		return false;
	}

	float entityWidth; // pixels
	float entityHeight; // pixels
	GameMap& gameMap;
};