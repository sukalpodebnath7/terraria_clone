#pragma once
#include <gameMap.h>
#include <vector>
using namespace std;



void countBg(vector<int>& bg);
int setBg(vector<int>& bg, GameMap &gameMap, int sX, int sY, int eX, int eY, int &val);