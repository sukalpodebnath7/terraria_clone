#pragma once
#include <assetManager.h>

void AssetManager::loadAll() {
	textures = LoadTexture(RESOURCES_PATH "texturesWithBackgroundVersion.png");
	forestBg = LoadTexture(RESOURCES_PATH "forestBG2.png");
	desertBg = LoadTexture(RESOURCES_PATH "desertBG2.png");
	snowBg = LoadTexture(RESOURCES_PATH "snowBG2.png");
	mountainBg = LoadTexture(RESOURCES_PATH "mountainBG2.png");
	texturesbg = LoadTexture(RESOURCES_PATH "texturesWithBackgroundVersion.png");
	frame = LoadTexture(RESOURCES_PATH "frame.png");
	player = LoadTexture(RESOURCES_PATH "playerSprites.png");
	zombie = LoadTexture(RESOURCES_PATH "zombie.png");
	caveBg = LoadTexture(RESOURCES_PATH "caveBg2.png");
	hearts = LoadTexture(RESOURCES_PATH "hearts.png");
	recipeBook = LoadTexture(RESOURCES_PATH "recipeBook.png");
	tools = LoadTexture(RESOURCES_PATH "items.png");
	//sounds
   blockBreak = LoadSound(RESOURCES_PATH "resources/sounds/break.ogg");
   place = LoadSound(RESOURCES_PATH "resources/sounds/place.ogg");
   playerAttack = LoadSound(RESOURCES_PATH "resources/sounds/bonus/Hit1.ogg");
   playerDamage = LoadSound(RESOURCES_PATH "resources/sounds/bonus/Hit2.ogg");
   chest = LoadSound(RESOURCES_PATH "resources/sounds/bonus/CheckBoxOn.ogg");
   enemyDead = LoadSound(RESOURCES_PATH"resources/sounds/FinalHit1.ogg");


   //music
   forestBG = LoadMusicStream(RESOURCES_PATH "resources/music/forest.ogg");
   snowBG = LoadMusicStream(RESOURCES_PATH "resources/music/snow");
   desertBG = LoadMusicStream(RESOURCES_PATH "resources/music/desert.ogg");
}
