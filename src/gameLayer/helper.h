#pragma once
#include <gameMap.h>
#include <vector>
#include <recipeBook.h>
#include <map>
#include <items.h>
#include <unordered_map>
#include <player.h>
#include <tools.h>
#include <raylib.h>
#include <randomFuncs.h>
#include <random>
#include <zombie.h>
using namespace std;



void countBg(vector<int>& bg);
int setBg(vector<int>& bg, GameMap &gameMap, int sX, int sY, int eX, int eY, int &val);
bool isBuilable(map<int, pair<Item, int>>& inventory, int tool);
void buildTool(map<int, pair<Item, int>>& inventory, int tool, GameMap& gameMap);

void updateInventory(map<int, pair<Item, int>>& inventory);

void toolFunction(int type, PlayerEntity& player, map<int, pair<Item, int>>& inventory, bool leftPress);

float getDamage(int tooltype);
