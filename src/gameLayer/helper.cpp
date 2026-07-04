#include <helper.h>
#include <block.h>

int setBg(vector<int>& bg, GameMap& gameMap, int sX, int sY, int eX, int eY, int &val) {

	for (int x = sX; x <= eX; x++) {
		for (int y = sY; y <= eY; y++) {
			if (gameMap.getBlock(x, y).type != Block::air) {
				int type = gameMap.getBlock(x, y).type;

				switch(type){
				case Block::grassBlock:
				case Block::dirt:
				case Block::grass:
					bg[0]++;
					break;
				case Block::stone:
					bg[1]++;
					break;
				case Block::sand:
				case Block::sandStone:
					bg[2]++;
					break;
				case Block::snow:
				case Block::ice:
					bg[3]++;
					break;
				default:
					break;
				}

				break;
			}
		}
	}


	int maxVal = bg[0];
	int maxIdx = 0;

	for (int i = 1; i < 4; i++) {
		if (bg[i] > maxVal) {
			maxVal = bg[i];
			maxIdx = i;
		}
	}

	val = maxVal;
	return maxIdx;


}



bool isBuilable(map<int, pair<Item, int>>& inventory, int tool) {
	vector<pair<int, int>>& itemRequired = recipeBook[tool];


	for (auto item : itemRequired) {
		bool isFound = false;
		int cnt;
		for (auto i : inventory) {
			if (i.second.first.type == item.first) {
				isFound = true;
				cnt = i.second.second;
			}
		}
		if (inventory.empty() || !isFound)
			return false;

		if (cnt < item.second) {
			return false;
		}
	}
	return true;
}



void buildTool(map<int, pair<Item, int>>& inventory, int tool, GameMap& gameMap) {
	vector<pair<int, int>>& itemRequired = recipeBook[tool];

	for (auto item : itemRequired) {
		for (auto& i : inventory) {
			if (i.second.first.type == item.first) {
				i.second.second -= item.second;
			}
		}
	}
	Item Tool{ gameMap };
	Tool.type = tool;
	
	inventory[tool] = { Tool, 1 };
}

void updateInventory(map<int, pair<Item, int>>& inventory) {
	vector<int> removed;
	for (auto i : inventory) {
		if (i.second.second == 0) {
			removed.push_back(i.first);
		}
	}

	for (auto i : removed) {
		inventory.erase(i);
	}
}