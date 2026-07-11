#pragma once

#include <raylib.h>

struct AssetManager {

	Texture2D textures;
	Texture2D forestBg;
	Texture2D texturesbg;
	Texture2D frame;
	Texture2D player;
	Texture2D zombie;
	Texture2D desertBg;
	Texture2D mountainBg;
	Texture2D snowBg;
	Texture2D caveBg;
	Texture2D hearts;
	Texture2D recipeBook;
	Texture2D tools;
<<<<<<< HEAD
	Texture2D mummy;
	Texture2D iceZombie;
=======
    //music
    Music forestBG;
    Music snowBG;
    Music desertBG;

   //sound
    Sound blockBreak;
    Sound place;
    Sound playerAttack;
    Sound playerDamage;
    Sound chest;
    Sound enemyDead;
>>>>>>> f87700f225a09011d83154fb2ee24d8a41934bcd
	void loadAll();
};
