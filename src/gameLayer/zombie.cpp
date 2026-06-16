#include <zombie.h>


void Zombie::entityBehaviour(Transform2D& playerTransform) {
	static int frameCount = 0;
	frameCount++;
	if (frameCount >= 64) {
		playerDetected = playerDetectionCheck(playerTransform);
	}
	bool shouldAttack = playerTransform.intersectTransform(transform) || playerTransform.intersectPoint(transform.pos);
	
	if (playerDetected) {
		if (shouldAttack) {
			state = Entity::attacking;
			velocity.x = 0;
		}
		else
			state = Entity::moving;
	}
	else {
		state = Entity::standing;
	}

	updateForces(deltaTime);
	checkCollisionOnce(gameMap, transform.pos);
	updateFinal();

	entityAction(playerTransform);
	
}


void Zombie::entityAction(Transform2D& player) {

	if (playerDetected) {

		if (state == Entity::moving) {
			float delta = player.pos.x - transform.pos.x;
			if (delta > 0)
				zombieMoving(player.getRight());
			else
				zombieMoving(player.getLeft());
		}
	}
}


void Zombie::zombieMoving(Vector2& target) {

	float delta = target.x - transform.pos.x;
	direction = delta < 0 ? -1 : 1;
	float speed = 7.f;
	float jumpSpeed = 12.5f;

	float nextX = transform.pos.x + direction*0.5f;
	if (gameMap.getBlock((int)nextX, (int)transform.pos.y).isCollidable()) {
		if (downTouch) {
			velocity.y = -jumpSpeed;
			velocity.x = speed * direction;
			state = Entity::jumping;
		}
	}
	else {
		velocity.x = speed*direction;
	} 


	

	
}
