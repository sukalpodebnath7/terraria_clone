#pragma once
#include <physics.h>
#include <gameMap.h>

struct Item : public PhysicalEntity {
	int type;
	GameMap* gameMap;

	Item() : gameMap(nullptr) {}

	Item(GameMap& gm) :
		gameMap(&gm)
	{}

	virtual ~Item() = default;

	void dropPrint(float deltaTime, Texture2D tex);
	void inHandPrint();
};