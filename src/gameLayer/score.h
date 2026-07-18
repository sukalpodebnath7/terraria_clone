#pragma once

struct Score {

	static constexpr int ENEMY_KILL_POINTS = 10;
	static constexpr int BOSS_KILL_POINTS  = 1000;

	int totalScore = 0;

	// Call when a regular enemy (zombie, mummy, ice-zombie) is killed
	void onEnemyKill();

	// Call when a boss is killed
	void onBossKill();

	// Reset score (e.g. on new game)
	void reset();
};
