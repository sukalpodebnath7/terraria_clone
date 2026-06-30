#pragma once
#include <entity.h>
#include <gameMap.h>
#include <player.h>

struct Zombie : public Entity {

	Zombie(GameMap& gm, PlayerEntity& pl) : Entity(gm, pl) {
		 entityWidth = 32;
		 entityHeight = 64;
		 attackDamage = 5.f;
	}

	void entityBehaviour(Transform2D& playerTrasform) override;
	void entityAction(Transform2D& player) override;
	void zombieMoving(Vector2& target);

	
	float deltaTime;
};