#include <score.h>

void Score::onEnemyKill() {
	totalScore += ENEMY_KILL_POINTS;
}

void Score::onBossKill() {
	totalScore += BOSS_KILL_POINTS;
}

void Score::reset() {
	totalScore = 0;
}
