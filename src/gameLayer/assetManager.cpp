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
}