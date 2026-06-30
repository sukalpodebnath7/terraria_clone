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
