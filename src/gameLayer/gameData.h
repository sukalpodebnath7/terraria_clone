#pragma once
#include <raylib.h>
#include <gameMap.h>
#include <structure.h>
#include <player.h>
#include <items.h>
#include <vector>
#include <map>
#include <zombie.h>

struct GameData {
	Camera2D camera;
	GameMap gameMap = {};

	int w = 5000;
	int h = 600;

	int selectedBlockType = Block::dirt;

	Vector2 selectionStart = { 0,0 };
	Vector2 selectionEnd = { 0,0 };

	Structure copyStructure;

	char saveName[100] = {};

	PlayerEntity player;
	std::vector<Item> dropedItems;
	std::map<int, std::pair<Item, int>> inventory;
	std::vector<int> bg;
	Texture2D prevBg;
};

extern GameData gameData;
extern std::vector<Zombie*> zombies;
