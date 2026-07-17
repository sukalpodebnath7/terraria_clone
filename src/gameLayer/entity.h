#pragma once
#include <raylib.h>
#include <physics.h>
#include <gameMap.h>
#include <iostream>
#include <player.h>
#include <helper.h>
using namespace std;

struct Entity : public PhysicalEntity {
	
	Entity(GameMap& gm, PlayerEntity& pl)
		: gameMap(gm), player(pl)
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

	inline void entityAttacked() {
		if (damageFrame >= 24) {
			if (player.state == PlayerEntity::attacking && player.transform.intersectTransform(transform)) {
				entityTakenDamage = getDamage(player.inHandBlock);
				entityHealthUpdate();
				entityTakenDamage = 0.f; 
				velocity.y = -5.f;
				downTouch = 0;
				velocity.x -= (direction == 1? 1 : -1)  * 30;

			}
			damageFrame = 0;
		}
		damageFrame++;
	}


	inline void entityHealthUpdate() {
		entityHealth = std::max(0.f, entityHealth - entityTakenDamage);
	}

	inline bool playerDetectionCheck(Transform2D& player) {
		
		float delX = abs(transform.pos.x - player.pos.x);
		float delY = abs(transform.pos.y - player.pos.y);

		if (delX <= 25.f && delY <= 25.f)
			return true;


		return false;
	}

	float entityWidth; // pixels
	float entityHeight; // pixels
	float attackDamage;
	GameMap& gameMap;
	PlayerEntity& player;
	int frameCount = 0;
	float entityHealth;
	float entityTakenDamage = 0.f;
	float damageFrame = 0;
	int  prevState = 0;
	int pos = 0;
	int frameCounter = 0;
};