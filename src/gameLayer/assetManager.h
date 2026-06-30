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

	void loadAll();
};