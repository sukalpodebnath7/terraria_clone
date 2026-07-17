#include <helper.h>
#include <block.h>
#include <audio.h>
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
	Audio::playSound(Audio::crafting);
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

void toolFunction(int type, PlayerEntity& player, map<int, pair<Item, int>>& inventory, bool leftPress) {
	if (type == Tool::Apple && inventory[type].second > 0) {
		if (!leftPress) {
			if (player.health[0] == 20.f) return;
			player.takenDamage -= 10.f;
			inventory[type].second--;
			Audio::playSound(Audio::eating);
		}
	}
}


float getDamage(int toolType)
{
	switch (toolType)
	{
		// Wood Tier (Damage: 5)
	case 70: // WoodPickaxe
	case 71: // WoodAxe
	case 72: // WoodHammer
	case 89: // WoodSword
		return 5;

		// Stone Tier (Damage: 7)
	case 73: // StonePickaxe
	case 74: // StoneAxe
	case 75: // StoneHammer
	case 90: // StoneSword
		return 7;

		// Copper Tier (Damage: 9)
	case 76: // CopperPickaxe
	case 78: // CopperHammer
	case 91: // CopperSword
	case 106: // CopperDagger
		return 9;

		// Iron Tier (Damage: 12)
	case 79: // IronPickaxe
	case 80: // IronAxe
	case 81: // IronHammer
	case 92: // IronSword
	case 108: // IronDagger
		return 12;

		// Gold / Elite Tier (Damage: 15)
	case 82: // GoldPickaxe
	case 83: // GoldAxe
	case 84: // GoldHammer
	case 93: // GoldSword
	case 107: // StoneDagger (Assuming high tier placement or 15)
		return 15;

		// Non-weapon/Utility Items (Damage: 0)
	case 100: // HealthPotion
	case 103: // Book
		return 0;

	default:
		return 0;
	}
}
