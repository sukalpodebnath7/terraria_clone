#pragma once
#include <entity.h>
#include <gameMap.h>


struct Zombie : public Entity {

	Zombie(GameMap& gm) : Entity(gm) {
		 entityWidth = 32;
		 entityHeight = 64;
	}

	void entityBehaviour(Transform2D& playerTrasform) override;
	void entityAction(Transform2D& player) override;
	void zombieMoving(Vector2& target);

	
	float deltaTime;
};