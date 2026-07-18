#include <saveGame.h>
#include <saveMap.h>
#include <gameData.h>
#include <raylib.h>
#include <fstream>
#include <assetManager.h>
#include <score.h>

extern AssetManager assetManager;
extern Score score;

void saveGame() {
	saveBlockDataToFile(gameData.gameMap.mapData, gameData.gameMap.w, gameData.gameMap.h, RESOURCES_PATH "savegame.bin");
	
	std::ofstream f(RESOURCES_PATH "saveplayer.bin", std::ios::binary);
	if (f.is_open()) {
		f.write((const char*)&gameData.player.transform.pos, sizeof(Vector2));
		
		size_t numZombies = zombies.size();
		f.write((const char*)&numZombies, sizeof(size_t));
		for (Zombie* z : zombies) {
			int texType = -1;
			if (z->entityTex.id == assetManager.mummy.id) texType = 0;
			else if (z->entityTex.id == assetManager.iceZombie.id) texType = 1;
			
			f.write((const char*)&texType, sizeof(int));
			f.write((const char*)&z->transform.pos, sizeof(Vector2));
			f.write((const char*)&z->entityHealth, sizeof(float));
		}
		
		// Save health
		f.write((const char*)gameData.player.health, sizeof(float) * 5);
		
		// Save score
		f.write((const char*)&score.totalScore, sizeof(int));
		
		// Save inventory
		size_t invSize = gameData.inventory.size();
		f.write((const char*)&invSize, sizeof(size_t));
		for (const auto& kv : gameData.inventory) {
			f.write((const char*)&kv.first, sizeof(int));
			f.write((const char*)&kv.second.first.type, sizeof(int));
			f.write((const char*)&kv.second.second, sizeof(int));
		}
		
		// Save dropped items
		size_t dropsSize = gameData.dropedItems.size();
		f.write((const char*)&dropsSize, sizeof(size_t));
		for (const auto& item : gameData.dropedItems) {
			f.write((const char*)&item.type, sizeof(int));
			f.write((const char*)&item.transform.pos, sizeof(Vector2));
		}
		
		f.close();
	}
}

void loadGame() {
	if (loadBlockDataFromFile(gameData.gameMap.mapData, gameData.gameMap.w, gameData.gameMap.h, RESOURCES_PATH "savegame.bin")) {
		// Reset entities and player after loading the map
		for (auto& z : zombies) {
			if (z != nullptr) delete z;
		}
		zombies.clear();
		gameData.dropedItems.clear();
		
		gameData.player.transform.pos = {100.5f, 200.5f};
		gameData.player.velocity = {0, 0};
		gameData.player.takenDamage = 0.f;

		std::ifstream f(RESOURCES_PATH "saveplayer.bin", std::ios::binary);
		if (f.is_open()) {
			f.read((char*)&gameData.player.transform.pos, sizeof(Vector2));
			
			size_t numZombies = 0;
			if (f.read((char*)&numZombies, sizeof(size_t))) {
				for (size_t i = 0; i < numZombies; i++) {
					int texType;
					Vector2 pos;
					float health;
					f.read((char*)&texType, sizeof(int));
					f.read((char*)&pos, sizeof(Vector2));
					f.read((char*)&health, sizeof(float));
					
					Zombie* z = new Zombie{ gameData.gameMap, gameData.player };
					z->transform.pos = pos;
					z->entityHealth = health;
					z->transform.w = 0.8f;
					z->transform.h = 1.8f;
					z->teleport(z->transform.pos);
					
					if (texType == 0) z->entityTex = assetManager.mummy;
					else if (texType == 1) z->entityTex = assetManager.iceZombie;
					else z->entityTex = assetManager.zombie;
					
					zombies.push_back(z);
				}
			}
			
			// Load health (with backwards compatibility check)
			if (f.read((char*)gameData.player.health, sizeof(float) * 5)) {
				// Load score
				f.read((char*)&score.totalScore, sizeof(int));
				
				// Load inventory
				gameData.inventory.clear();
				size_t invSize = 0;
				if (f.read((char*)&invSize, sizeof(size_t))) {
					for (size_t i = 0; i < invSize; ++i) {
						int key, type, count;
						f.read((char*)&key, sizeof(int));
						f.read((char*)&type, sizeof(int));
						f.read((char*)&count, sizeof(int));
						
						Item item;
						item.type = type;
						item.gameMap = &gameData.gameMap;
						gameData.inventory[key] = {item, count};
					}
				}
				
				// Load dropped items
				gameData.dropedItems.clear();
				size_t dropsSize = 0;
				if (f.read((char*)&dropsSize, sizeof(size_t))) {
					for (size_t i = 0; i < dropsSize; ++i) {
						int type;
						Vector2 pos;
						f.read((char*)&type, sizeof(int));
						f.read((char*)&pos, sizeof(Vector2));
						
						Item item;
						item.type = type;
						item.gameMap = &gameData.gameMap;
						item.transform.pos = pos;
						item.transform.w = 0.5f;
						item.transform.h = 0.5f;
						item.teleport(pos);
						gameData.dropedItems.push_back(item);
					}
				}
			} else {
				// Backward compatibility if saving an old file
				for (int i = 0; i < 5; i++) {
					gameData.player.health[i] = 20.f;
				}
				score.totalScore = 0;
				gameData.inventory.clear();
				gameData.dropedItems.clear();
			}
			
			f.close();
		} else {
			gameData.player.transform.pos = {100.5f, 200.5f};
			for (int i = 0; i < 5; i++) {
				gameData.player.health[i] = 20.f;
			}
			score.totalScore = 0;
			gameData.inventory.clear();
			gameData.dropedItems.clear();
		}
	}
}
