#pragma once
#include <raylib.h>


namespace Audio {

	void init();

	void loadAllMusicAndSounds();

	void update();

	void playSound(int sound, float volume = 1);

	enum {
		noneSound = 0,
		placeBlock,
		breakBlock,
		takenDamage,
		clickSound,
		backButtonPress,
		eating,
		zombie,
		entityDeath,
		crafting,
		attcking,


		SOUNDS_COUNT
	};

}